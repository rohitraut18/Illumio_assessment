#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const string FLOW_LOG_FILE = "flow_logs.txt";
const string LOOKUP_FILE = "lookup_table.csv";
const string OUTPUT_FILE = "output.txt";

unordered_map<string, string> LOOKUP;
unordered_map<string, string> PROTOCOLS = {{"1", "icmp"}, {"6", "tcp"}, {"17", "udp"}};

void create_lookup_table(const string& lookup_file) 
{
    ifstream file(lookup_file);
    if (!file.is_open()) 
    {
        cerr << "Error opening file: " << lookup_file << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) 
    {
        stringstream ss(line);
        string dstport_str, protocol, tag;
        getline(ss, dstport_str, ',');
        getline(ss, protocol, ',');
        getline(ss, tag, ',');
        try 
        {
            for (char &ch : protocol) 
            {
                ch = tolower(ch);
            }
            string x = dstport_str + " " + protocol;
            LOOKUP[x] = tag;
        } 
        catch (const invalid_argument& e) 
        {
            cerr << "Invalid port number: " << dstport_str << endl;
            continue;
        }
    }
    file.close();
}

void parse_flow_logs(const string& flow_log_file, unordered_map<string, int>& tag_counts, unordered_map<string, int>& port_protocol_counts) {
    ifstream file(flow_log_file);
    if (!file.is_open()) 
    {
        cerr << "Error opening file: " << flow_log_file << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> fields;
        string field;
        while (getline(ss, field, ' ')) 
        {
            fields.push_back(field);
        }
        try 
        {
            string dstport = fields[6]; 
            string protocol_number = fields[7];
            string protocol = PROTOCOLS.count(protocol_number) ? PROTOCOLS[protocol_number] : "unknown";
            string key = dstport + " " + protocol;
            string tag = LOOKUP.count(key) ? LOOKUP[key] : "untagged";
            tag_counts[tag]++;
            port_protocol_counts[key]++;
        } 
        catch (const invalid_argument& e) 
        {
            cerr << "Invalid port" << endl;
            continue;
        }
    }

    file.close();
}

void write_output(unordered_map<string, int>& tag_counts, unordered_map<string, int>& port_protocol_counts) 
{
    ofstream output_file(OUTPUT_FILE);
    if (!output_file.is_open()) 
    {
        cerr << "Error opening file: " << OUTPUT_FILE << endl;
        exit(EXIT_FAILURE);
    }
    output_file << "Tag Counts: \n";
    output_file << "Tag,Count\n";

    for (auto &entry : tag_counts)
    {
        output_file << entry.first << "," << entry.second << "\n";
    }
    output_file << endl;
    output_file << "Port/Protocol Combination Counts: \n";
    output_file << "Port,Protocol,Count\n";
    for (const auto& entry : port_protocol_counts) 
    {
        stringstream ss(entry.first);
        string word = "";
        ss >> word;
        string port = word;
        ss >> word;
        string protocol = word;
        output_file << port << "," << protocol << "," << entry.second << "\n";
    }
    output_file.close();
}

int main() {
    string flow_log_file = FLOW_LOG_FILE;
    string lookup_file = LOOKUP_FILE;

    create_lookup_table(lookup_file);

    unordered_map<string, int> tag_counts;
    unordered_map<string, int> port_protocol_counts;

    parse_flow_logs(flow_log_file, tag_counts, port_protocol_counts);

    write_output(tag_counts, port_protocol_counts);

    return 0;
}
