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

x_positions = np.array([-140, -100, -60, -20, 20, 60, 100, 140])
y_positions = np.array([-140, -100, -60, -20, 20, 60, 100, 140])
layer_depth = np.array([-140, -100, -60, -20, 20, 60, 100, 140])

##	triggered channels
V_signal = SimpleSiPM.PhotonCount()

trigger = V_signal * [V_signal > 0.05]

##	Ref: Hamamatsu S13360-2050VE datasheet spec
M = 1.7e6
N_pixels = 1583
C_terminal = 140e-12
C_cell = C_terminal / N_pixels

##	electron charge in Coulombs
e = 1.6e-19

dV = M * e / C_cell

N_photons = V_signal * N_pixels / dV

photon_MeV = 2.7e-6

e_light = 0.023
e_coll = 0.041
e_quantum = 0.373

energies = N_photons * photon_MeV / (e_light * e_coll * e_quantum)

print(energies)

hits_3d = []
hits_inter =[]
hits_E = []

for layer in range(8):
	x_start = layer * 16 + 8
	y_start = layer * 16

	Ex = energies[x_start:x_start+8]
	Ey = energies[y_start:y_start+8]

	x_fired = np.where(energies[x_start:x_start+8])[0]
	y_fired = np.where(energies[y_start:y_start+8])[0]

	if (len(x_fired) > 0) & (len(y_fired) > 0):
		hits_3d.append((
			np.average(x_positions[x_fired], weights=Ex[x_fired]),
			np.average(y_positions[y_fired], weights=Ey[y_fired]),
			layer_depth[layer]
		))

		hits_E.append(np.sum(Ex) + np.sum(Ey))



for layer in range(8):
	x_start = layer * 16 + 8
	y_start = layer * 16

	x_fired = np.where(energies[x_start:x_start+8])[0]
	y_fired = np.where(energies[y_start:y_start+8])[0]

	for xi in x_fired:
		for yi in y_fired:
			hits_inter.append((x_positions[xi], y_positions[yi], layer_depth[layer]))



plt.style.use('seaborn-v0_8-whitegrid')

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

fit = FitTrackSVD(hits_3d)

print(fit)
vec = np.array([np.sin(fit[1]) * np.cos(fit[2]), np.sin(fit[1]) * np.sin(fit[2]), np.cos(fit[1])])
line = np.array([fit[0] + 3e2 * vec, fit[0] - 3e2 * vec])
ax.plot(line[:, 0], line[:, 1], line[:, 2], ":", c='black', lw=1.5, alpha=0.5)

hits = np.array(hits_3d)
hits2 = np.array(hits_inter)

ax.scatter(hits2[:, 0], hits2[:, 1], hits2[:, 2], s=15, c='black', alpha=0.5)

scat = ax.scatter(hits[:, 0], hits[:, 1], hits[:, 2], s=np.array(hits_E)**4/5, c=hits_E, cmap='viridis')

cbar = fig.colorbar(scat, ax=ax, pad=0.1, shrink=0.9)
cbar.set_label('Hit cluster energy [MeV]')

ax.set_xlabel('X [mm]')
ax.set_ylabel('Y [mm]')
ax.set_zlabel('Z [mm]')

ax.set_xlim(-200, 200)
ax.set_ylim(-200, 200)
ax.set_zlim(-200, 200)

ax.set_box_aspect([1, 1, 1])

plt.show()