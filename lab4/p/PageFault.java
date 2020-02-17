/* It is in this file, specifically the replacePage function that will
   be called by MemoryManagement when there is a page fault.  The 
   users of this program should rewrite PageFault to implement the 
   page replacement algorithm.
*/

  // This PageFault file is an example of the FIFO Page Replacement 
  // Algorithm as described in the Memory Management section.

package p;

import java.util.*;

public class PageFault {

  /**
   * The page replacement algorithm for the memory management sumulator.
   * This method gets called whenever a page needs to be replaced.
   * <p>
   * The page replacement algorithm included with the simulator is 
   * FIFO (first-in first-out).  A while or for loop should be used 
   * to search through the current memory contents for a canidate 
   * replacement page.  In the case of FIFO the while loop is used 
   * to find the proper page while making sure that virtPageNum is 
   * not exceeded.
   * <pre>
   *   Page page = ( Page ) mem.elementAt( oldestPage )
   * </pre>
   * This line brings the contents of the Page at oldestPage (a 
   * specified integer) from the mem vector into the page object.  
   * Next recall the contents of the target page, replacePageNum.  
   * Set the physical memory address of the page to be added equal 
   * to the page to be removed.
   * <pre>
   *   controlPanel.removePhysicalPage( oldestPage )
   * </pre>
   * Once a page is removed from memory it must also be reflected 
   * graphically.  This line does so by removing the physical page 
   * at the oldestPage value.  The page which will be added into 
   * memory must also be displayed through the addPhysicalPage 
   * function call.  One must also remember to reset the values of 
   * the page which has just been removed from memory.
   *
   * @param mem is the vector which contains the contents of the pages 
   *   in memory being simulated.  mem should be searched to find the 
   *   proper page to remove, and modified to reflect any changes.  
   * @param virtPageNum is the number of virtual pages in the 
   *   simulator (set in Kernel.java).  
   * @param replacePageNum is the requested page which caused the 
   *   page fault.  
   * @param controlPanel represents the graphical element of the 
   *   simulator, and allows one to modify the current display.
   */

  private static int lastTouchTime(Clock clock, int id ){
    return clock.getI(id).lastTouchTime;
  }


  private static int chosePage(Vector mem, int virtPageNum, int tau, Clock clock){
    Page page;
    int countWritten=10;
    boolean wasChange=false;
    int oldestPage=-1;
    int i=32;
    //for(int i=0;i<(1+wasChange)*clock.size();i++)
    while (i!=0){
      i--;
      page = clock.getNext();

      if (oldestPage==-1)
      oldestPage=clock.getIndex();
        else if (lastTouchTime(clock,clock.getIndex())>lastTouchTime(clock, oldestPage))
        oldestPage=clock.getIndex();

      if (page.M==0){

        if(page.lastTouchTime>=tau)
          if(page.R==1){
            if (!wasChange)
              i=i+32;
            wasChange=true;
            page.R=0;
          }
          else {
            System.out.println(">tau. R=0");
            return clock.getIndex();
          }
      }
      else {

          if(countWritten>0&&page.lastTouchTime>=tau ){
            if (!wasChange)
              i=i+32;
            wasChange=true;
            countWritten--;
            page.M=0;
            page.R=0;
          }    
        }
    }


    if (wasChange){
      System.out.print("was change but wasnt output. error");
      return -1;
      
    }
    else { 
      return oldestPage;
    }
    

  }


  public static void replacePage ( Vector mem , int virtPageNum , int replacePageNum , ControlPanel controlPanel, int tau, Clock clock)
  {

    int oldestPage = chosePage(mem, virtPageNum, tau, clock);
    System.out.println(oldestPage);
    Page page =clock.getI(oldestPage);


    Page nextpage = ( Page ) mem.elementAt( replacePageNum );
    controlPanel.removePhysicalPage( clock.getRealIndex(oldestPage));
    nextpage.physical = page.physical;
    controlPanel.addPhysicalPage( nextpage.physical , replacePageNum );
    page.inMemTime = 0;
    page.lastTouchTime = 0;
    page.R = 0;
    page.M = 0;
    page.physical = -1;
    clock.set(oldestPage, nextpage.id);
  }
}
