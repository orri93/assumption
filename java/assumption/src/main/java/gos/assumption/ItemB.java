package gos.assumption;

public class ItemB extends Item {
  private static final long serialVersionUID = 1863773174241372283L;
  
  private int value;
  
  public ItemB() {
  	value = 0;
  }
  
  public ItemB(String name, int value) {
  	super(name);
  	this.value = value;
  }

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}
}
