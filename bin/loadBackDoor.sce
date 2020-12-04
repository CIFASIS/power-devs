if atomsIsInstalled("BackDoor")==%F then 
    rep=atomsRepositoryList();
    if strindex(rep(1),"5.4")~=[] 
        atomsRepositoryDel('http://atoms.scilab.org/5.4');
        atomsRepositoryAdd('http://atoms.scilab.org/5.5');
    end 
	disp("Installing BackDoor module");
	atomsInstall("BackDoor");
	PDEVS_SCILAB_HOME="/opt/powerdevs/3rd-party/scilab-5.5.2/share/scilab/contrib/BackDoor/0.2/etc/"
	movefile(PDEVS_SCILAB_HOME+"BackDoor.quit",PDEVS_SCILAB_HOME+"BackDoor.quit.bk")
	copyfile(PDEVS_SCILAB_HOME+"BackDoor.quit.bk",PDEVS_SCILAB_HOME+"backdoor.quit")
	movefile(PDEVS_SCILAB_HOME+"BackDoor.start",PDEVS_SCILAB_HOME+"BackDoor.start.bk")
	copyfile(PDEVS_SCILAB_HOME+"BackDoor.start.bk",PDEVS_SCILAB_HOME+"backdoor.start")
end 
atomsLoad("BackDoor");
