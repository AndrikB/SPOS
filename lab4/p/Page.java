package p;

public class Page 
{
  public int id;
  public int physical;
  public byte R;
  public byte M;
  public int inMemTime;
  public int lastTouchTime;
  public int lastModTime;
  public long high;
  public long low;

  public Page( int id, int physical, byte R, byte M, int inMemTime, int lastTouchTime,int lastModTime, long high, long low ) 
  {
    this.id = id;
    this.physical = physical;
    this.R = R;
    this.M = M;
    this.inMemTime = inMemTime;
    this.lastTouchTime = lastTouchTime;
    this.lastModTime=lastModTime;
    this.high = high;
    this.low = low;
  } 	

}
