{ "add", "roa ...", "Add ROA record", 0 },
{ "add roa", "<prefix> max <num> as <num> [table <name>]", "Add ROA record", 1 },
{ "configure check", "[\"<file>\"]", "Parse configuration and check its validity", 1 },
{ "configure confirm", "", "Confirm last configuration change - deactivate undo timeout", 1 },
{ "configure", "[\"<file>\"] [timeout [<sec>]]", "Reload configuration", 1 },
{ "configure", "...", "Reload configuration", 0 },
{ "configure soft", "[\"<file>\"] [timeout [<sec>]]", "Reload configuration and ignore changes in filters", 1 },
{ "configure soft timeout", "[<sec>]", "Reload configuration with undo timeout", 1 },
{ "configure timeout", "[<sec>]", "Reload configuration with undo timeout", 1 },
{ "configure undo", "", "Undo last configuration change", 1 },
{ "debug", "...", "Control protocol debugging via BIRD logs", 0 },
{ "debug", "(<protocol> | <pattern> | all) (all | off | { states | routes | filters | interfaces | events | packets })", "Control protocol debugging via BIRD logs", 1 },
{ "delete", "roa ...", "Delete ROA record", 0 },
{ "delete roa", "<prefix> max <num> as <num> [table <name>]", "Delete ROA record", 1 },
{ "disable", "<protocol> | \"<pattern>\" | all", "Disable protocol", 1 },
{ "down", "", "Shut the daemon down", 1 },
{ "dump attributes", "", "Dump attribute cache", 1 },
{ "dump", "...", "Dump debugging information", 0 },
{ "dump events", "", "Dump event log", 1 },
{ "dump interfaces", "", "Dump interface information", 1 },
{ "dump neighbors", "", "Dump neighbor cache", 1 },
{ "dump protocols", "", "Dump protocol information", 1 },
{ "dump resources", "", "Dump all allocated resource", 1 },
{ "dump routes", "", "Dump routing table", 1 },
{ "dump sockets", "", "Dump open sockets", 1 },
{ "echo", "(all | off | { debug | trace | info | remote | warning | error | auth }) [<buffer-size>]", "Control echoing of log messages", 1 },
{ "echo", "...", "Control echoing of log messages", 0 },
{ "enable", "<protocol> | \"<pattern>\" | all", "Enable protocol", 1 },
{ "eval", "<expr>", "Evaluate an expression", 1 },
{ "exit", "", "Exit the client", 1 },
{ "flush", "roa [table <name>]", "Removes all dynamic ROA records", 0 },
{ "flush roa", "[table <name>]", "Removes all dynamic ROA records", 1 },
{ "help", "", "Description of the help system", 1 },
{ "mrtdump", "...", "Control protocol debugging via MRTdump files", 0 },
{ "mrtdump", "(<protocol> | <pattern> | all) (all | off | { states | messages })", "Control protocol debugging via MRTdump format", 1 },
{ "quit", "", "Quit the client", 1 },
{ "reload in", "<protocol> | \"<pattern>\" | all", "Reload protocol (just imported routes)", 1 },
{ "reload out", "<protocol> | \"<pattern>\" | all", "Reload protocol (just exported routes)", 1 },
{ "reload", "<protocol> | \"<pattern>\" | all", "Reload protocol", 1 },
{ "restart", "<protocol> | \"<pattern>\" | all", "Restart protocol", 1 },
{ "restrict", "", "Restrict current CLI session to safe commands", 1 },
{ "show bfd sessions", "[<name>]", "Show information about BFD sessions", 1 },
{ "show bfd", "...", "Show information about BFD protocol", 0 },
{ "show interfaces", "", "Show network interfaces", 1 },
{ "show interfaces summary", "", "Show summary of network interfaces", 1 },
{ "show memory", "", "Show memory usage", 1 },
{ "show ospf interface", "[<name>] [\"<interface>\"]", "Show information about interface", 1 },
{ "show ospf lsadb", "[global | area <id> | link] [type <num>] [lsid <id>] [self | router <id>] [<proto>]", "Show content of OSPF LSA database", 1 },
{ "show ospf lsadb", "...", "Show content of OSPF LSA database", 0 },
{ "show ospf", "[<name>]", "Show information about OSPF protocol XXX", 1 },
{ "show ospf neighbors", "[<name>] [\"<interface>\"]", "Show information about OSPF neighbors", 1 },
{ "show ospf", "...", "Show information about OSPF protocol", 0 },
{ "show ospf state all", "[<name>]", "Show information about all OSPF network state", 1 },
{ "show ospf state", "[all] [<name>]", "Show information about OSPF network state", 0 },
{ "show ospf state", "[<name>]", "Show information about reachable OSPF network state", 1 },
{ "show ospf topology all", "[<name>]", "Show information about all OSPF network topology", 1 },
{ "show ospf topology", "[all] [<name>]", "Show information about OSPF network topology", 0 },
{ "show ospf topology", "[<name>]", "Show information about reachable OSPF network topology", 1 },
{ "show protocols all", "[<protocol> | \"<pattern>\"]", "Show routing protocol details", 1 },
{ "show protocols", "[<protocol> | \"<pattern>\"]", "Show routing protocols", 1 },
{ "show roa", "[<prefix> | in <prefix> | for <prefix>] [as <num>] [table <t>]", "Show ROA table", 1 },
{ "show roa", "...", "Show ROA table", 0 },
{ "show route", "[<prefix>|for <prefix>|for <ip>] [table <t>] [filter <f>|where <cond>] [all] [primary] [filtered] (export|preexport|noexport) <p>] [protocol <p>] [stats|count]", "Show routing table", 1 },
{ "show route", "...", "Show routing table", 0 },
{ "show", "...", "Show status information", 0 },
{ "show static", "[<name>]", "Show details of static protocol", 1 },
{ "show status", "", "Show router status", 1 },
{ "show symbols", "...", "Show all known symbolic names", 0 },
{ "show symbols", "[table|filter|role_map|function|protocol|template|roa|<symbol>]", "Show all known symbolic names", 1 },