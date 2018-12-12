package gos.assumption;

import java.io.Serializable;

public class Item implements Serializable {
  private static final long serialVersionUID = 3145387489217601872L;
  
  private String name;
  
  public Item() {
  	this.name = null;
  }
  
  public Item(String name) {
  	this.name = name;
  }

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}
