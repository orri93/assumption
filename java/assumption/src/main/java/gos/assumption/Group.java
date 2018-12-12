package gos.assumption;

import java.io.Serializable;

public class Group implements Serializable {
  private static final long serialVersionUID = -7961726924232057999L;
  
  private Type type;

	public Type getType() {
		return type;
	}

	public void setType(Type type) {
		this.type = type;
	}
}
