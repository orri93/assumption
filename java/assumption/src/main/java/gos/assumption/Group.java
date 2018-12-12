package gos.assumption;

import java.io.Serializable;

public class Group implements Serializable {
  private static final long serialVersionUID = -7961726924232057999L;
  
  private Type type;
  private String id;
  
  public Group() {
  	this.type = null;
  	this.id = null;
  }
  
  public Group(Type type) {
  	this.type = type;
  	this.id = null;
  }
  
  public Group(Type type, String id) {
  	this.type = type;
  	this.id = id;
  }

	public Type getType() {
		return type;
	}

	public void setType(Type type) {
		this.type = type;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}
}
