package gos.assumption;

public class GroupB extends Group {
  private static final long serialVersionUID = 4110606060480748245L;

  private ItemB item;
  
  public GroupB() {
  	super(Type.B);
  	this.item = null;
  }
  
  public GroupB(String id, ItemB item) {
    super(Type.B, id);
    this.item = item;
  }

	public ItemB getItem() {
		return item;
	}

	public void setItem(ItemB item) {
		this.item = item;
	}
}
