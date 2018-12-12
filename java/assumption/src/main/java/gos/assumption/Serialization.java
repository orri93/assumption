package gos.assumption;

import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;

public class Serialization {
	public void Serialize(Group group, OutputStream stream) throws Exception {
		Type type = group.getType();
		switch (type) {
		case A:
			if (group instanceof GroupA) {
				ObjectOutputStream output = new ObjectOutputStream(stream);
				GroupA groupa = (GroupA)group;
				output.writeObject(groupa);
			} else {
				throw new Exception("Group is not a instance of GroupA");
			}
		case B:
			if (group  instanceof GroupB) {
				ObjectOutputStream output = new ObjectOutputStream(stream);
				GroupB groupb = (GroupB)group;
				output.writeObject(groupb);
			} else {
				throw new Exception("Group is not a instance of GroupB");
			}
		default:
			throw new Exception(
			    "Unable to serialize group of type " + type.toString());
		}
	}
	
	public Group Deserialize(InputStream stream) throws Exception {
		return null;
	}
}
