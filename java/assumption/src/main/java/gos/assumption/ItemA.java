package gos.assumption;

public class ItemA extends Item {
  private static final long serialVersionUID = 7540799050371240704L;
  
  private String value;
  
  public ItemA() {
  	value = null;
  }
  
  public ItemA(String name, String value) {
  	super(name);
  	this.value = value;
  }

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}
}
