/* v4l2_capture.c – Minimal V4L2 single-frame capture example (public SDK).
 *
 * Captures one RAW frame from /dev/video0 (Shakti MIPI CSI-2 subdevice
 * pipeline) using memory-mapped buffers and writes it to a file.
 *
 * Build: gcc -O2 -o v4l2_capture v4l2_capture.c
 * Usage: ./v4l2_capture [/dev/video0] [width] [height] [out.raw]
 *
 * License: Apache-2.0 (see ../../LICENSE).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

#define DIE(msg) do { perror(msg); return 1; } while (0)

int main(int argc, char **argv) {
    const char *dev = (argc > 1) ? argv[1] : "/dev/video0";
    int width  = (argc > 2) ? atoi(argv[2]) : 640;
    int height = (argc > 3) ? atoi(argv[3]) : 480;
    const char *out = (argc > 4) ? argv[4] : "frame.raw";

    int fd = open(dev, O_RDWR);
    if (fd < 0) DIE("open");

    /* Discard the first frame: it may be partial after programming
     * (see docs/known_issues.md K-PUB-001). We stream 2 buffers and
     * keep the second. */
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) DIE("VIDIOC_S_FMT");

    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = 2;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) DIE("VIDIOC_REQBUFS");
    if (req.count < 2) { fprintf(stderr, "need 2 buffers\n"); return 1; }

    void *map[2]; unsigned mlen[2];
    for (int i = 0; i < 2; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) DIE("VIDIOC_QUERYBUF");
        mlen[i] = buf.length;
        map[i] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, buf.m.offset);
        if (map[i] == MAP_FAILED) DIE("mmap");
        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) DIE("VIDIOC_QBUF");
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) DIE("VIDIOC_STREAMON");

    /* Dequeue twice: drop frame 0 (K-PUB-001), keep frame 1. */
    struct v4l2_buffer buf;
    for (int i = 0; i < 2; i++) {
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) DIE("VIDIOC_DQBUF");
        if (i == 0 && ioctl(fd, VIDIOC_QBUF, &buf) < 0) DIE("VIDIOC_QBUF");
    }

    FILE *f = fopen(out, "wb");
    if (!f) DIE("fopen");
    size_t n = fwrite(map[buf.index], 1, buf.bytesused, f);
    fclose(f);
    printf("wrote %zu bytes (%ux%u) to %s\n", n, width, height, out);

    ioctl(fd, VIDIOC_STREAMOFF, &type);
    for (int i = 0; i < 2; i++) munmap(map[i], mlen[i]);
    close(fd);
    return (n == 0);
}
