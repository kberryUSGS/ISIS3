APPNAME = isisui
NOPREFERENCES = true

include $(ISISROOT)/make/isismake.tsts

commands:
	$(APPNAME) gradient \
	  from=$(INPUT)/isisTruth.cub \
	  to=isisTruthSample.cub \
	  gradtype=sobel \
	  method=approximate  > $(OUTPUT)/standardOutTruth2.pvl;
