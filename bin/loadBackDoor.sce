if atomsIsInstalled("BackDoor")==%F then 
    rep=atomsRepositoryList();
    if strindex(rep(1),"5.4")~=[] 
        atomsRepositoryDel('http://atoms.scilab.org/5.4');
        atomsRepositoryAdd('http://atoms.scilab.org/5.5');
    end 
	disp("Installing BackDoor module");
	atomsInstall("BackDoor");
	movefile(SCIHOME+"/atoms/BackDoor/0.2/etc/BackDoor.quit",SCIHOME+"/atoms/BackDoor/0.2/etc/BackDoor.quit.bk")
	copyfile(SCIHOME+"/atoms/BackDoor/0.2/etc/BackDoor.quit.bk",SCIHOME+"/atoms/BackDoor/0.2/etc/backdoor.quit")
	movefile(SCIHOME+"/atoms/BackDoor/0.2/etc/BackDoor.start",SCIHOME+"/atoms/BackDoor/0.2/etc/BackDoor.start.bk")
	copyfile(SCIHOME+"/atoms/BackDoor/0.2/etc/BackDoor.start.bk",SCIHOME+"/atoms/BackDoor/0.2/etc/backdoor.start")
end 
atomsLoad("BackDoor");
