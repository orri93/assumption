package gos.assumption;

public class GroupA extends Group {
  private static final long serialVersionUID = -1838705511925082950L;

  private ItemA item;
  
  public GroupA(ItemA item) {
    super.setType(Type.A);
    this.item = item;
  }

	public ItemA getItem() {
		return item;
	}

	public void setItem(ItemA item) {
		this.item = item;
	}
}
