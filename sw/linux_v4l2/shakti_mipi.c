/* shakti_mipi.c – Linux V4L2 subdev driver for MIPI-CSI2 AI Camera on Shakti Ganga (RV64)
 * Replaces sw/src/v4l2_subdev.c bare-metal; targets Ganga MMU + media controller.
 * Build against Shakti Linux (PetaLinux) with bsp-ganga device tree.
 * Address map: see docs/register_map.md (0x44A0_0000 region).
 */
#include <linux/module.h>
#include <media/v4l2-subdev.h>
#include <media/media-entity.h>

#define DRV_NAME "shakti-mipi-csi2"
#define REG_BASE 0x44A00000 /* example – from Address Editor */

struct shakti_mipi {
  struct v4l2_subdev sd;
  struct media_pad pad;
  void __iomem *regs;
};

static int shakti_mipi_s_stream(struct v4l2_subdev *sd, int enable) {
  struct shakti_mipi *mipi = container_of(sd, struct shakti_mipi, sd);
  // LP→HS, enable hard PHY (MC20901) via GPIO, start DMA (see DMA DT node)
  iowrite32(enable ? 1 : 0, mipi->regs + 0x00);
  return 0;
}

static const struct v4l2_subdev_video_ops video_ops = {
  .s_stream = shakti_mipi_s_stream,
};
static const struct v4l2_subdev_ops ops = { .video = &video_ops };

static int shakti_mipi_probe(struct platform_device *pdev) {
  struct shakti_mipi *mipi;
  mipi = devm_kzalloc(&pdev->dev, sizeof(*mipi), GFP_KERNEL);
  mipi->regs = devm_ioremap_resource(&pdev->dev,
    platform_get_resource(pdev, IORESOURCE_MEM, 0));
  v4l2_subdev_init(&mipi->sd, &ops);
  mipi->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
  mipi->pad.flags = MEDIA_PAD_FL_SOURCE;
  return v4l2_device_register_subdev(&mipi->sd);
}
static const struct of_device_id shakti_mipi_of_match[] = {
  { .compatible = "shakti,mipi-csi2-ganga" }, {} // see sw/include/mipi_csi2_dt.h
};
MODULE_DEVICE_TABLE(of, shakti_mipi_of_match);
static struct platform_driver shakti_mipi_driver = {
  .driver = { .name = DRV_NAME, .of_match_table = shakti_mipi_of_match },
  .probe = shakti_mipi_probe,
};
module_platform_driver(shakti_mipi_driver);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Shakti Ganga MIPI CSI-2 AI Camera V4L2 subdev (MC20901 hard PHY)");
