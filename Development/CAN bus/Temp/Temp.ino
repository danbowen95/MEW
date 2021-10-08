int can_id;
int axis_id;
int cmd_id;
int can_id_calc;

void setup()
{
  Serial.begin(115200);
  axis_id = 0x01;
  cmd_id = 0x0c;
  can_id = 0x01 << 5 | 0x0C;
  can_id_calc = axis_id << 5 | cmd_id;
}

void loop()
{
  Serial.print("axis_id: ");
  Serial.println(axis_id);
  Serial.print("cmd_id: ");
  Serial.println(cmd_id);
  Serial.print("can_id (calculated): ");
  Serial.println(can_id_calc);
  Serial.print("can_id (correct): ");
  Serial.println(can_id);
  Serial.println("* --- *");
  delay(5000);
}
