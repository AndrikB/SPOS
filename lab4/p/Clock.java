package p;

import java.util.*;

public class Clock{

    private int currentPage=0;
    private Vector<Integer> physicalPages;
    private Vector mem;

Clock( Vector<Integer> physicalPages, Vector mem){
    this.mem=mem;
    this.physicalPages=physicalPages;
}
    public Page getNext(){
        currentPage=(currentPage+1)%physicalPages.size();
        return getCurrent();
    } 

    public Page getCurrent(){
        return (Page)mem.elementAt(physicalPages.elementAt(currentPage));
    }

    public Page getI(int i){
        return (Page)mem.elementAt(physicalPages.elementAt(i));
    }

    public int getIndex(){
        return currentPage;
    }

    public int getRealIndex(){
        return physicalPages.elementAt(currentPage);
    }

    public int getRealIndex(int i){
        return physicalPages.elementAt(i);
    }

    public int size(){
        return physicalPages.size();
    }

    public void set(int i, int element){
        physicalPages.set(i, element);
    }

}