// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.
package p;

import java.util.Vector;
import java.io.*;
import p.*;
public class SchedulingAlgorithm {

  private static boolean isComplited(sProcess process){
    return process.cpudone==process.cputime;
  }
  
  private static boolean allComplited(Vector<sProcess> processVector, int blocked){
    int size =processVector.size();
    for(int i=0;i<size;i++){
      if (i==blocked) continue;
      if (!isComplited(processVector.elementAt(i)))
        return false;
    }
    return true;
  }

  private static int choseNextGroup(Vector<Vector<sProcess>> groupProcess, int currentGroop){
    do{
      currentGroop=(currentGroop+1)%groupProcess.size();
    }while(allComplited(groupProcess.elementAt(currentGroop), -1));
    return currentGroop;
  }

  private static int choseNextProcess(Vector<sProcess> processVector,  int currentProcess, int blocked ){
    do{
      currentProcess=(currentProcess+1)%processVector.size();
    }while(isComplited(processVector.elementAt(currentProcess))||currentProcess==blocked);
    return currentProcess;
  }

  private static int countNotComplited(Vector<sProcess> processVector){
    int count=0;
    int size =processVector.size();
    for(int i=0;i<size;i++){
      if (!isComplited(processVector.elementAt(i)))
        count++;
    }
    return count;
  }

  public static Results Run(int runtime, Vector<sProcess> processVector, Results result) {
    int i = 0;
    int comptime = 0;
    int currentGroop=0;
    int size = processVector.size();//count all process
    int groupSize=(int)Math.sqrt(size);
    int completed = 0;//count of completed process
    final int quantum=100;
    int timeLastChangeGroup=0;
    int timeLastChangeProcess=0;
    String resultsFile = "Summary-Processes";

    Vector<Vector<sProcess>> groupProcess= new Vector<Vector<sProcess>>();
    Vector<Integer> numberProcess=new Vector<>();
    for(i=0;i<groupSize; i++){
      groupProcess.add(new Vector<sProcess>());
      numberProcess.add(0);
    }
      
    
    for(i=0;i<size;i++){
      int j;
      j=i%groupSize;
      groupProcess.elementAt(j).add(processVector.elementAt(i));
    }

    result.schedulingType = "Batch (Nonpreemptive)";
    result.schedulingName = "First-Come First-Served"; 


    try {
      //BufferedWriter out = new BufferedWriter(new FileWriter(resultsFile));
      //OutputStream out = new FileOutputStream(resultsFile);
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = groupProcess.elementAt(currentGroop).elementAt(numberProcess.elementAt(currentGroop));
      out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop) + " registered... \t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
	  
      while (comptime < runtime) {//not timeout

        if (process.cpudone == process.cputime) {//is completed
          completed++;
          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop) + " completed... \t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          if (completed == size) {//all complited
            result.compuTime = comptime;
            out.close();
            return result;
          }
          if (allComplited(groupProcess.elementAt(currentGroop),-1)){
            currentGroop=choseNextGroup(groupProcess, currentGroop);
            timeLastChangeGroup=0;
            timeLastChangeProcess=0;
          }
          else{
            numberProcess.set(currentGroop, choseNextProcess(groupProcess.elementAt(currentGroop),numberProcess.elementAt(currentGroop),-1));
          }
          process=groupProcess.elementAt(currentGroop).elementAt(numberProcess.elementAt(currentGroop));
          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop) + " registered... \t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
        } 
    
        
        if (process.ioblocking == process.ionext) {//block process
          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop) + " I/O blocked...\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          process.numblocked++;//count blocking THIS process
          process.ionext = 0;

            timeLastChangeProcess=0;
          if (allComplited(groupProcess.elementAt(currentGroop),numberProcess.elementAt(currentGroop))){
            currentGroop=choseNextGroup(groupProcess, currentGroop);
            timeLastChangeGroup=0;
          }
          else{
            numberProcess.set(currentGroop, choseNextProcess(groupProcess.elementAt(currentGroop),numberProcess.elementAt(currentGroop),numberProcess.elementAt(currentGroop)));
          }
          process=groupProcess.elementAt(currentGroop).elementAt(numberProcess.elementAt(currentGroop));

          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop)+ " registered... \t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
        }

        if (timeLastChangeProcess==quantum/countNotComplited(groupProcess.elementAt(currentGroop))){
          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop)+ " stoped... \t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          numberProcess.set(currentGroop, choseNextProcess(groupProcess.elementAt(currentGroop),numberProcess.elementAt(currentGroop),-1));
          process=groupProcess.elementAt(currentGroop).elementAt(numberProcess.elementAt(currentGroop));
          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop)+ " started... \t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          timeLastChangeProcess=0;
        }


        if (timeLastChangeGroup==quantum){
          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop)+ " stoped... \t\t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          currentGroop=choseNextGroup(groupProcess, currentGroop);
          process=groupProcess.elementAt(currentGroop).elementAt(numberProcess.elementAt(currentGroop));
          out.println("Process: " + currentGroop+'.'+ numberProcess.elementAt(currentGroop)+ " started... \t(" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          timeLastChangeGroup=0;
          timeLastChangeProcess=0;
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
