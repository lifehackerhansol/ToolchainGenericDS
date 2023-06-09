all: tgdsBuild

tgdsBuild: 
	$(MAKE)	-R	-C	ntr/
	$(MAKE)	-R	-C	twl/
	
clean:
	$(MAKE) -C ntr/ clean
	$(MAKE) -C twl/ clean
	$(MAKE) -C lib/ clean
	
#---------------------------------------------------------------------------------

commitChanges:
	-@git commit -a	-m '$(COMMITMSG)'
	-@git push origin HEAD
	
#---------------------------------------------------------------------------------

switchStable:
	-@git checkout -f	'TGDS1.65'
	
#---------------------------------------------------------------------------------

switchMaster:
	-@git checkout -f	'master'
