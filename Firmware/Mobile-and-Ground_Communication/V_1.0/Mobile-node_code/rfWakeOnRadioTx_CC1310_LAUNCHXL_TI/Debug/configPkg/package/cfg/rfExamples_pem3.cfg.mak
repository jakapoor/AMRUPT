# invoke SourceDir generated makefile for rfExamples.pem3
rfExamples.pem3: .libraries,rfExamples.pem3
.libraries,rfExamples.pem3: package/cfg/rfExamples_pem3.xdl
	$(MAKE) -f /Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI/src/makefile.libs

clean::
	$(MAKE) -f /Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI/src/makefile.libs clean

