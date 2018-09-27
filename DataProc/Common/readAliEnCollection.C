void readAliEnCollection(const char *in, const char *out) {
  TXMLEngine *xml = new TXMLEngine();
  XMLDocPointer_t doc = xml->ParseFile(in);
  XMLNodePointer_t root = xml->DocGetRootElement(doc);  // <alien>
  XMLNodePointer_t coll = xml->GetChild(root);  // <collection>
  if (!coll || !out || strcmp(xml->GetNodeName(coll), "collection")) {
    gSystem->Exit(1);
  }
  ofstream ofp;
  ofp.open(out);
  for (XMLNodePointer_t evt=xml->GetChild(coll); evt; evt=xml->GetNext(evt)) { // <event>
    if (strcmp(xml->GetNodeName(evt), "event")) continue;
    XMLNodePointer_t file = xml->GetChild(evt);  // <file>
    if (strcmp(xml->GetNodeName(file), "file")) continue;
    ofp << xml->GetAttr(file, "turl") << endl;
  }
  ofp.close();
}
