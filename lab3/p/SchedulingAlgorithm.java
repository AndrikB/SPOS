// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.
package p;

import java.util.Vector;


import java.io.*;
import p.*;
public class SchedulingAlgorithm {

  static boolean isComplited(sProcess process){
    return process.cpudone==process.cputime;
  }
  
  private static int choseNextGroup(Vector<Group> groups, int currentGroop){
    do{
      currentGroop=(currentGroop+1)%groups.size();
    }while(groups.elementAt(currentGroop).countNotCompleted==0);
    return currentGroop;
  }


  public static Results Run(int runtime, Vector<sProcess> processVector, Results result, int quant) {
    int i = 0;
    int comptime = 0;
    int currentGroup=-1;
    int size = processVector.size();//count all process
    int completed = 0;//count of completed process
    int timeLastChangeGroup=0;
    int timeLastChangeProcess=0;
    int countNotCompleted;
    String resultsFile = "Summary-Processes";

    int groupSize=1;
    for(i=0;i<size;i++){
      if(groupSize<=processVector.elementAt(i).numberGroup)
      groupSize=processVector.elementAt(i).numberGroup+1;
    }

    Vector<Group> groups=new Vector<>();

    for(i=0;i<groupSize; i++){
      groups.add(new Group(new Vector<sProcess>()));
    }
    for(i=0;i<size;i++){
      groups.elementAt(processVector.elementAt(i).numberGroup).processes.add(processVector.elementAt(i));
      groups.elementAt(processVector.elementAt(i).numberGroup).countNotCompleted++;
    }

    result.schedulingType = "Batch (preemptive)";
    result.schedulingName = "Fair-Share"; 


    try {
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      currentGroup=choseNextGroup(groups, currentGroup);//becouse 0 group can have no one process
      countNotCompleted=groups.elementAt(currentGroup).countNotCompleted;
      sProcess process = groups.elementAt(currentGroup).getCurrentProcess();
      out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess + " registered... \t\t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
	  
      while (comptime < runtime) {//not timeout

        if (process.cpudone == process.cputime) {//is completed
          completed++;
          groups.elementAt(currentGroup).countNotCompleted--;
          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess + " completed... \t\t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          if (completed == size) {//all complited
            result.compuTime = comptime;
            out.close();
            return result;
          }
          if (groups.elementAt(currentGroup).countNotCompleted==0){
            currentGroup=choseNextGroup(groups, currentGroup);
            countNotCompleted=groups.elementAt(currentGroup).countNotCompleted;
            timeLastChangeGroup=0;
            timeLastChangeProcess=groups.elementAt(currentGroup).timeLastProcessWorked;
            groups.elementAt(currentGroup).timeLastProcessWorked=0;
          }
          else{
            groups.elementAt(currentGroup).choseNextProcess();
          }
          process = groups.elementAt(currentGroup).getCurrentProcess();
          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess + " started... \t\t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
        } 
    
        
        if (process.ioblocking == process.ionext) {//block process
          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess + " I/O blocked...\t\t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          process.numblocked++;//count blocking THIS process
          process.ionext = 0;

          if (groups.elementAt(currentGroup).countNotCompleted==1){//1 is blocked
            currentGroup=choseNextGroup(groups, currentGroup);
            countNotCompleted=groups.elementAt(currentGroup).countNotCompleted;
            timeLastChangeGroup=0;
          }
          else{
            groups.elementAt(currentGroup).choseNextProcess();
          }
          timeLastChangeProcess=0;
          process = groups.elementAt(currentGroup).getCurrentProcess();

          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess+ " started... \t\t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
        }


        if (timeLastChangeProcess==quant/countNotCompleted){
          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess+ " stoped,end process time(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          groups.elementAt(currentGroup).choseNextProcess();
          process=groups.elementAt(currentGroup).getCurrentProcess();
          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess+ " started... \t\t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          timeLastChangeProcess=0;
        }

        
        if (timeLastChangeGroup==quant){
          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess+ " stoped, end group time\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          groups.elementAt(currentGroup).timeLastProcessWorked=timeLastChangeProcess%(quant/countNotCompleted);//save time
          currentGroup=choseNextGroup(groups, currentGroup);
          countNotCompleted=groups.elementAt(currentGroup).countNotCompleted;
          process=groups.elementAt(currentGroup).getCurrentProcess();
          out.println("Process: " + currentGroup+'.'+ groups.elementAt(currentGroup).currentProcess+ " started... \t\t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          timeLastChangeGroup=0;
          timeLastChangeProcess=groups.elementAt(currentGroup).timeLastProcessWorked;
        }
        



        process.cpudone++;       
        if (process.ioblocking > 0) {
          process.ionext++;
        }
        comptime++;
        timeLastChangeGroup++;
        timeLastChangeProcess++;
      }


      out.close();
    } catch (IOException e) { /* Handle exceptions */ }
    result.compuTime = comptime;
    return result;
  }
}
