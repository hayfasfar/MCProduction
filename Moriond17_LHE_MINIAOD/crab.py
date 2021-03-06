from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'PrivateMCProduction_amarini_August17_MH125'
config.General.workArea = 'crab_privateMCProduction'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'PrivateMC'
#config.JobType.disableAutomaticOutputCollection = True
config.JobType.maxMemoryMB = 2500
config.JobType.psetName = 'MINIAODSIM.py' ## fake the last step
config.JobType.inputFiles = ['scriptExe.sh', 'GEN.py','MINIAODSIM.py','pu.py','step1.py','step3.py']
config.JobType.scriptExe='scriptExe.sh'
config.JobType.numCores=4

config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 500
config.Data.totalUnits = 200000
config.Data.outLFNDirBase = '/store/group/phys_higgs/cmshmm/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputPrimaryDataset = 'GluGluHToMuMu_M125_13TeV_amcatnloFXFX_pythia8'
config.Data.outputDatasetTag ='PUMoriond17-MINIAODSIM-v2'

config.Site.storageSite = 'T2_CH_CERN'

