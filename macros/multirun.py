import subprocess
import numpy as np

scintLength = 320   # mm
scintWidth  = 40    # mm
pointsPerCm = 4

spacing = 10 / pointsPerCm  # 2.5 mm

x = np.linspace(-scintLength/2, scintLength/2, int(scintLength / spacing) + 1)
y = np.linspace(-scintLength/2, scintWidth/2,  int(scintWidth  / spacing) + 1)

for xi in x:
    for yi in y:
        subprocess.call([
            "./vcortex_sim",
            "-t", "8",
            "-o", f"dataTD/output_mu_{yi}_{xi}_.csv",
            "-p", str(xi), "15", str(yi),
            "-d", "0", "-1", "0",
            "-P", "mu-",
            "-e", "1000",
            "one.mac"
        ], stdout=subprocess.DEVNULL)