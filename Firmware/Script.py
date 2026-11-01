import time
import psutil
import serial
import pynvml

PORT = "COM3"
BAUD = 115200

gpu_handle = None
try:
    pynvml.nvmlInit()
    gpu_handle = pynvml.nvmlDeviceGetHandleByIndex(0)
except Exception:
    pass

try:
    with serial.Serial(PORT, BAUD, timeout=1) as ser:
        time.sleep(3)
        while True:
            cpu = int(psutil.cpu_percent(interval=1))
            ram = int(psutil.virtual_memory().percent)
            gpu = 0
            vram = 0

            if gpu_handle is not None:
                try:
                    usage = pynvml.nvmlDeviceGetUtilizationRates(gpu_handle)
                    memory = pynvml.nvmlDeviceGetMemoryInfo(gpu_handle)
                    gpu = int(usage.gpu)
                    if memory.total > 0:
                        vram = int(memory.used * 100 / memory.total)
                except Exception:
                    pass

            payload = f"{cpu},{ram},{gpu},{vram}\n"
            ser.write(payload.encode("utf-8"))
            print(payload.strip())
except serial.SerialException as error:
    print("Could not open " + PORT + ": " + str(error))
except KeyboardInterrupt:
    print("Disconnected safely.")
