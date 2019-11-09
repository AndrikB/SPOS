// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.
package p;

import java.util.Vector;
import java.io.*;
import p.*;
public class SchedulingAlgorithm {

  public static Results Run(int runtime, Vector<sProcess> processVector, Results result) {
    int i = 0;
    int comptime = 0;
    int currentProcess = 0;
    int previousProcess = 0;
    int size = processVector.size();//count all process
    int completed = 0;//count of completed process
    String resultsFile = "Summary-Processes";

    result.schedulingType = "Batch (Nonpreemptive)";
    result.schedulingName = "First-Come First-Served"; 
    try {
      //BufferedWriter out = new BufferedWriter(new FileWriter(resultsFile));
      //OutputStream out = new FileOutputStream(resultsFile);
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = processVector.elementAt(currentProcess);
      out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
	  
      while (comptime < runtime) {//not timeout
	  
        if (process.cpudone == process.cputime) {//is completed
          completed++;
          out.println("Process: " + currentProcess + " completed... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
          if (completed == size) {//all complited
            result.compuTime = comptime;
            out.close();
            return result;
          }
          for (i = size - 1; i >= 0; i--) {
            process = processVector.elementAt(i);
            if (process.cpudone < process.cputime) { //chose first no complited
              currentProcess = i;
            }
          }
          process = processVector.elementAt(currentProcess);//chose next process
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
        }      
		
        if (process.ioblocking == process.ionext) {//block process
          out.println("Process: " + currentProcess + " I/O blocked... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
          process.numblocked++;//count blocking THIS process
          process.ionext = 0; 
          previousProcess = currentProcess;
          for (i = size - 1; i >= 0; i--) {
            process = processVector.elementAt(i);
            if (process.cpudone < process.cputime && previousProcess != i) { //chose first no complited and no blocked just now
              currentProcess = i;
            }
          }
          process = processVector.elementAt(currentProcess);
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
        }        
        process.cpudone++;       
        if (process.ioblocking > 0) {
          process.ionext++;
        }
        comptime++;
      }
      out.close();
    } catch (IOException e) { /* Handle exceptions */ }
    result.compuTime = comptime;
    return result;
  }
}
