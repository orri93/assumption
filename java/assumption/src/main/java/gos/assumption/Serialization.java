package gos.assumption;

import java.io.Writer;
import java.util.Map;
import org.yaml.snakeyaml.DumperOptions.FlowStyle;
import org.yaml.snakeyaml.Yaml;
import org.yaml.snakeyaml.constructor.Constructor;
import org.yaml.snakeyaml.nodes.Tag;

public class Serialization {

	private static final String TypeText = "type";

	public void Serialize(Group group, Writer writer) throws Exception {
		Type type = group.getType();
		Yaml yaml = new Yaml();
		switch (type) {
		case A:
			if (group instanceof GroupA) {
				GroupA groupa = (GroupA) group;
				yaml.dump(groupa, writer);
			} else {
				throw new Exception("Group is not a instance of GroupA");
			}
			break;
		case B:
			yaml.dump(group, writer);
			break;
		default:
			throw new Exception(
			    "Unable to serialize group of type " + type.toString());
		}
	}

	public String Serialize(Group group) throws Exception {
		Type type = group.getType();
		Yaml yaml = new Yaml();
		switch (type) {
		case A:
			if (group instanceof GroupA) {
				GroupA groupa = (GroupA) group;
				return yaml.dumpAs(groupa, Tag.MAP, FlowStyle.AUTO);
			} else {
				throw new Exception("Group is not a instance of GroupA");
			}
		case B:
			return yaml.dumpAs(group, Tag.MAP, FlowStyle.AUTO);
		default:
			throw new Exception(
			    "Unable to serialize group of type " + type.toString());
		}
	}

	public Group Deserialize(String input) throws Exception {
		Type type = DeserializeType(input);
		//TypeDescription descriptionType = new TypeDescription(Type.class);
		//TypeDescription description;
		Constructor constructor;
		switch (type) {
		case A:
			//input = "!!gos.assumption.GroupA\n" + input;
			//description = new TypeDescription(ItemA.class);
			constructor = new Constructor(GroupA.class);
			//constructor.addTypeDescription(description);
			break;
		case B:
			//input = "!!gos.assumption.GroupB\n" + input;
			//description = new TypeDescription(ItemB.class);
			constructor = new Constructor(GroupB.class);
			//constructor.addTypeDescription(description);
			break;
		default:
			throw new Exception(
			    "Unable to deserialize group of type " + type.toString());
		}
		//constructor.addTypeDescription(descriptionType);
		Yaml yaml = new Yaml(constructor);
		return yaml.load(input);
	}

	private Type DeserializeType(String input) {
		Yaml yaml = new Yaml();
		Map<String, Object> map = yaml.load(input);
		if (map.containsKey(TypeText)) {
			Object object = map.get(TypeText);
			if (object instanceof String) {
				return Type.valueOf((String) object);
			}
		}
		return null;
	}

}
