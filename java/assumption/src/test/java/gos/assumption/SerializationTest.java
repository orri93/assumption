package gos.assumption;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;

import org.junit.jupiter.api.Test;

public class SerializationTest {

  @Test
  public void testSerialize() throws Exception {
    ItemA item = new ItemA();
    GroupA group = new GroupA(item);
    Serialization serialization = new Serialization();
    ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
    serialization.Serialize(group, outputStream);
    String output =
        new String(outputStream.toByteArray(), StandardCharsets.UTF_8);
    assertEquals("", output);
  }

  @Test
  public void testDeserialize() {

  }

}
