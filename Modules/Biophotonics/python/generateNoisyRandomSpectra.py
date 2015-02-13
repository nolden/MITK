# -*- coding: utf-8 -*-
"""
Created on Fri Feb  6 16:14:57 2015

@author: wirkert
"""

import time
import datetime
import random

import numpy as np

import monteCarloHelper as mch
import setupSimulation as setup


# the input file without the run specific parameters for ua, us and d:
infileString = 'input/darmpythontest.mci'
infile       = open(infileString)
# the output folder for the mc simulations
outfolderMC = 'outputMC/'
# the output folder for the reflectance spectra
outfolderRS = 'outputRS/'


BVFs, Vss, ds, SaO2s, rs, nrSamples, photons, wavelengths, FWHM, eHbO2, eHb = setup.setupNormalSimulation()


nrSimulations = 100
reflectances  = np.zeros((nrSimulations, len(wavelengths)))
parameters    = np.zeros((nrSimulations, 8))

print('start simulations...')

#%% start program logic
start = time.time()



for i in range(nrSimulations):
    j = 0
    print('starting simulation ' + str(i) + ' of ' + str(nrSimulations))

    BVF = random.uniform(min(BVFs), max(BVFs))
    Vs  = random.uniform(min(Vss), max(Vss))
    d   = random.uniform(min(ds), max(ds))
    r   = random.uniform(min(rs), max(rs))
    SaO2= random.uniform(min(SaO2s), max(SaO2s))

    sm_BVF = random.uniform(min(BVFs), max(BVFs))
    sm_Vs  = random.uniform(min(Vss), max(Vss))
    sm_SaO2= random.uniform(min(SaO2s), max(SaO2s))



    parameters[i,:] = np.array([BVF, Vs, d, r, SaO2, sm_BVF, sm_Vs, sm_SaO2])


    for wavelength in wavelengths:

        reflectanceValue = mch.runOneSimulation(
            wavelength, eHbO2, eHb,
            infile, outfolderMC,
            BVF, Vs, d,
            r, SaO2,
            submucosa_BVF=sm_BVF, submucosa_Vs=sm_Vs, submucosa_SaO2=sm_SaO2,
            Fwhm = FWHM, nrPhotons=photons)


        print((BVF, Vs, d, SaO2, r, wavelength, sm_BVF, sm_Vs, sm_SaO2))

        # here, summarize result from wavelength in reflectance spectrum
        reflectances[i, j] = reflectanceValue
        j +=1




infile.close()

# save the reflectance results!
now = datetime.datetime.now().strftime("%Y%B%d%I:%M%p")
np.save(outfolderRS + now + "reflectancesRandomWithNoise", reflectances)
np.save(outfolderRS + now + "paramterersRandomWithNoise", parameters)

end = time.time()
print "total time for script: " + str((end - start))