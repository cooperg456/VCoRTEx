import sys
import ROOT
import numpy as np





##	---------------------------------------------------------------------------
##	ROOT file to Numpy array
##	---------------------------------------------------------------------------

_hc_eVnm = 1239.841984

def PhotonHitsFromROOT(filename: str, run: int, event: int) -> list[np.ndarray]:
	"""
	Extract photon hit wavelengths from a ROOT file for a single event.

	Reads photon energies from the specified tree and converts them to wavelengths in nm via λ = 2πħc / E. 
	Returns multiply arrays of photon wavelengths where each row corresponds to the detector sharing its index
	"""
	eventData = ROOT.RDataFrame(f"run{run}", filename).AsNumpy(["energy_eV", "detID", "eventID"])

	eventMask = eventData["eventID"] == event

	eventHits = [
		_hc_eVnm / eventData["energy_eV"][eventMask & (eventData["detID"] == i)]
		for i in range(np.max(eventData["detID"]) + 1)
	]

	return eventHits



##	---------------------------------------------------------------------------
##	Photon Detection Efficiency — Hamamatsu S13360-2050VE MPPC
##	---------------------------------------------------------------------------

_PDE_wl_nm = np.array([320, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710, 720, 730, 740, 750, 760, 770, 780, 790, 800, 810, 820, 830, 840, 850, 860, 870, 880, 890, 900])
_PDE_eff = np.array([0.032299, 0.086746, 0.158448, 0.215951, 0.250831, 0.282414, 0.313005, 0.338190, 0.356299, 0.369444, 0.382573, 0.389871, 0.395496, 0.400830, 0.402269, 0.399872, 0.396907, 0.393233, 0.388736, 0.377727, 0.366841, 0.354596, 0.341854, 0.326502, 0.314021, 0.301696, 0.289328, 0.275034, 0.261708, 0.250655, 0.238924, 0.222553, 0.208097, 0.197737, 0.188599, 0.179829, 0.168568, 0.159434, 0.152775, 0.142841, 0.135013, 0.127185, 0.119432, 0.112202, 0.105005, 0.098222, 0.091974, 0.086246, 0.080678, 0.075045, 0.069880, 0.064684, 0.059458, 0.054830, 0.050182, 0.045472, 0.041361, 0.037233, 0.03511])

def PhotonDetectionEfficiency(wl_nm) -> np.ndarray:
	"""
	Photon detection efficiency curve for Hamamatsu S13360-2050VE MPPC.

	PDE values digitized from figure on page 3 of datasheet using WebPlotDigitizer (https://automeris.io/)

	Datasheet: Hamamatsu Photonics, "S13360-2050VE". 
	https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/s13360-2050ve_etc_kapd1053e.pdf

	Archived: 
	https://web.archive.org/web/20250710004058/https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/s13360-2050ve_etc_kapd1053e.pdf
	"""

	return np.interp(wl_nm, _PDE_wl_nm, _PDE_eff)



##	load hits from geant4 output
hits = PhotonHitsFromROOT(
	sys.argv[1], 		##	filename.root
	int(sys.argv[2]), 	##	run number
	int(sys.argv[3])	##	event number
)



##	---------------------------------------------------------------------------
##	SiPM and Trigger simulation
##	---------------------------------------------------------------------------

def PhotonCount():
	##	Applying photon detection efficiency PDE(λ) to each photon hit to approximate number of registered photons
	N_det = np.array([
		np.sum(np.random.rand(len(hits[i])) < PhotonDetectionEfficiency(hits[i]))
		for i in range(len(hits))
	])

	##	Ref: Hamamatsu S13360-2050VE datasheet spec
	M = 1.7e6
	N_pixels = 1583
	C_terminal = 140e-12
	C_cell = C_terminal / N_pixels

	##	electron charge in Coulombs
	e = 1.6e-19

	##	Saturation correction, Eq. 2-13
	##	Ref: A. Ghassemi, K. Sato, K. Kobayashi, "Technical guide to silicon photomultipliers (MPPC)",
	##		Hamamatsu Photonics, KAPD9005E, Nov. 2022.
	##	https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/mppc_kapd9005e.pdf
	N_fired = (N_pixels * (1 - np.exp(-N_det / N_pixels)))

	##	Overvoltage per fired microcell, Pg. 12
	##	Ref: A. Ghassemi, K. Sato, K. Kobayashi, "Technical guide to silicon photomultipliers (MPPC)",
	##		Hamamatsu Photonics, KAPD9005E, Nov. 2022
	dV = M * e / C_cell

	##	Charge per fired microcell
	##Q_total = N_fired * (C_cell * dV)

	##	Signal voltage per fired microcell
	V_signal = dV * N_fired / N_pixels

	return V_signal