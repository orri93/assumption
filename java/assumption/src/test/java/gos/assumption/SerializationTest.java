package gos.assumption;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.StringWriter;
import java.nio.charset.StandardCharsets;

import org.junit.jupiter.api.Test;

public class SerializationTest {

  @Test
  public void testSerialize() throws Exception {
  	ItemA itema;
  	ItemB itemb;
  	Group group;
  	Serialization serialization;
  	StringWriter writer;
  	String output, expected;
    
    serialization = new Serialization();
    
  	itema = new ItemA("A", "Item A");
    group = new GroupA("123", itema);
    writer = new StringWriter();
    serialization.Serialize(group, writer);
    output = writer.toString();
    expected = "!!gos.assumption.GroupA\n" + 
    		"id: '123'\n" + 
    		"item: {name: A, value: Item A}\n" + 
    		"type: A\n";
    assertEquals(expected, output);
    
    itemb = new ItemB("B", 93);
    group = new GroupB("345", itemb);
    output = serialization.Serialize(group);
    expected = "id: '345'\n" + 
    		"item: {name: B, value: 93}\n" + 
    		"type: B\n";
    assertEquals(expected, output);
  }

  @Test
  public void testDeserialize() throws Exception {
  	ItemB itemb;
  	ItemA itema;
  	Group group;
  	GroupB groupb;
  	GroupA groupa;
  	Serialization serialization;
  	String input;
    
    serialization = new Serialization();
    input = "id: '345'\n" + 
    		"item: {name: B, value: 93}\n" + 
    		"type: B\n";
    group = serialization.Deserialize(input);
    assertNotNull(group);
    assertEquals(Type.B, group.getType());
    assertEquals("345", group.getId());
    assertTrue(group instanceof GroupB);
    groupb = (GroupB)group;
    itemb = groupb.getItem();
    assertEquals("B", itemb.getName());
    assertEquals(93, itemb.getValue());
    
    
    input = "id: 11\n" +
    	  "type: A\n" +
    		"item:\n" +
    		"  name: A\n" +
    		"  value: 93\n";
    group = serialization.Deserialize(input);
    assertNotNull(group);
    assertEquals(Type.A, group.getType());
    assertEquals("11", group.getId());
    assertTrue(group instanceof GroupA);
    groupa = (GroupA)group;
    itema = groupa.getItem();
    assertEquals("A", itema.getName());
    assertEquals("93", itema.getValue());
  }

}
