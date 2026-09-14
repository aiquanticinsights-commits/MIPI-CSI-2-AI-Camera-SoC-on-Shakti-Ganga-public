# Architecture Overview (Public)

## Dataflow

```
Camera (D-PHY) --> MC20901 bridge --> CSI-2 RX --> VC merge --+
                                                              +--> ISP --> AI preproc --> DMA --> DDR
Camera (C-PHY/event) --> packet layer (CSI-3/ESP) ------------+
```

The image path is: MIPI physical link -> link/packet layer ->
aggregation -> ISP -> AI preprocessing, supervised by the Shakti
Ganga core (I2C camera config, DMA, UART, GPIO, timers, interrupts).

## Blocks (functional view)

| Block | Function |
|-------|----------|
| D-PHY bridge (MC20901) | Hard MIPI D-PHY, 1-4 lanes |
| CSI-2 RX | SOT/EOT, packet headers (VC, datatype, word count), frame/line sequencing, error counters |
| CSI-3 / ESP | Packet transport, sparse/event data, multi-sensor fusion |
| VC merge | Merges sensor streams by virtual channel |
| LR merge / stitching | Line-rate merge (HDR), multi-sensor stitching |
| ISP | Demosaic, AWB, AE, ANR, color correction, gamma |
| AI preproc | Resize, color convert, INT8 quantize, pack |
| Shakti Ganga | RV64IMAC control plane: I2C, DMA scheduling, V4L2, GPIO, watchdog |

## Interfaces seen by software

- AXI4-Lite register file per block (see `docs/register_map.md`)
- AXI4-Stream pixel transport between blocks, AXI DMA to DDR
- PLIC interrupt on frame done / error; I2C for sensor registers

## Clocking / reset

- 100 MHz board clock; camera byte clock from the D-PHY bridge
- Active-low async reset throughout; per-block enable registers
