import matplotlib.pyplot as plt
import numpy as np

import SimpleSiPM as SimpleSiPM





##	---------------------------------------------------------------------------
##	Track reconstruction and plotting
##	---------------------------------------------------------------------------

def FitTrackSVD(points):
    centroid = np.mean(points, axis=0)
    centered = points - centroid

    _, _, vh = np.linalg.svd(centered)
    direction = vh[0] 

    dx, dy, dz = direction
    norm = np.linalg.norm(direction)

    theta = np.arccos(dz / norm)  
    phi = np.arctan2(dy, dx)      

    if np.round(theta, 2) == np.round(np.pi, 2):
        theta = 0

    return (centroid, theta, phi)

x_positions = [-140, -100, -60, -20, 20, 60, 100, 140]
y_positions = [-140, -100, -60, -20, 20, 60, 100, 140]
layer_depth = [-140, -100, -60, -20, 20, 60, 100, 140]

hits_3d = []

##	triggered channels
trigger = SimpleSiPM.PhotonCount() > 0.05

for layer in range(8):
	x_start = layer * 16 + 8
	y_start = layer * 16

	x_fired = np.where(trigger[x_start:x_start+8])[0]
	y_fired = np.where(trigger[y_start:y_start+8])[0]

	for xi in x_fired:
		for yi in y_fired:
			hits_3d.append((x_positions[xi], y_positions[yi], layer_depth[layer]))



plt.style.use('seaborn-v0_8-whitegrid')

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

fit = FitTrackSVD(hits_3d)

print(fit)
vec = np.array([np.sin(fit[1]) * np.cos(fit[2]), np.sin(fit[1]) * np.sin(fit[2]), np.cos(fit[1])])
line = np.array([fit[0] + 3e2 * vec, fit[0] - 3e2 * vec])
ax.plot(line[:, 0], line[:, 1], line[:, 2], "r:", lw=1.5, alpha=0.8)

hits = np.array(hits_3d)
ax.scatter(hits[:, 0], hits[:, 1], hits[:, 2], s=75, c='steelblue')

ax.set_xlabel('X [mm]')
ax.set_ylabel('Y [mm]')
ax.set_zlabel('Z [mm]')

ax.set_box_aspect([1, 1, 1])

plt.show()