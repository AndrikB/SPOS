package p;

import java.io.*;
import java.util.*;

import p.*;
public class Group{
    public Vector<sProcess> processes;
    public int currentProcess;
    public int timeLastProcessWorked;
    public int countNotCompleted;

    public Group(Vector<sProcess> processes){
        this.processes=processes;
        currentProcess=0;
        timeLastProcessWorked=0;
        countNotCompleted=processes.size();
    }

    public sProcess getCurrentProcess(){
        return processes.elementAt(currentProcess);
    }

    public void choseNextProcess(){
        do{
            currentProcess=(currentProcess+1)%processes.size();
          }while(SchedulingAlgorithm.isComplited(processes.elementAt(currentProcess)));
    }

}