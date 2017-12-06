ExportXMLbackground(const Char_t *xmlname)
{
  TAlienCollection coll(xmlname, 1);
  coll.Reset();
  coll.Next();
  printf("export CONFIG_BACKGROUND=%s\n", coll.GetTURL());
}
