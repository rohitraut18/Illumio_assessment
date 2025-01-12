# Flow Log Analyzer

## Overview
This program processes network flow logs (`flow_logs.txt`) and a lookup table (`lookup_table.csv`) to generate two reports:
1. **Tag Counts**: A summary of how many times each tag appears.
2. **Port/Protocol Combination Counts**: A summary of counts for each unique destination port and protocol combination.

# Assumptions

1. **Default Input Format**  
   - The program assumes that the `flow_logs.txt` and `lookup_table.csv` files are in the required formats as described in the project specifications. Any deviation from these formats might lead to parsing errors or incorrect outputs.
   - The input flow logs follow the default log format: Space-separated fields, with the destination port at index 6 and the protocol number at index 7.
   - Only version 2 logs are supported.
   - The lookup table (`lookup_table.csv`) is a CSV file with the following columns:
     - `dstport` (destination port)
     - `protocol`
     - `tag`
   - The program only supports well-known protocol numbers (1, 6, 17) for ICMP, TCP, and UDP, respectively for now. All other protocols are labeled as `unknown`.

2. **Input/Output Accuracy**  
   - The given input (`flow_logs.txt` and `lookup_table.csv`) and output (`output.txt`) may not align perfectly if there are inconsistencies. For example:
     - If the lookup table contains a mapping for `22, tcp` but the flow log file does not have any entries corresponding to `22, tcp`, this combination will not appear in the output. 
     - Similarly, if the flow log file contains data that does not match any tag in the lookup table, it will be labeled as `untagged`.

3. **File Placement**  
   - Both `flow_logs.txt` and `lookup_table.csv` files must be placed in the same directory as the program executable to be processed correctly.

4. **Supported Log Versions**  
   - The program is designed to process version 2 flow logs. Other log versions are not supported or tested.

5. **Handling Invalid Input**  
   - Any invalid or corrupt lines in the lookup table or flow log file are ignored, and the program proceeds with the valid data.

6. **Unrecognized Protocol Numbers**  
   - Any protocol number not present in the `PROTOCOLS` dictionary will default to `unknown` in the output.

6. **Tag Mapping Requirement**  
   - Tags are mapped only when both the port and protocol combination exists in both the lookup table and the flow log file.

---

## Requirements
- C++17 or later.

## Compilation
Use the following command to compile the program:
```bash
g++ -std=c++17 -o main main.cpp
```

## Execution

Run the program with the following files in the same directory:

- `flow_logs.txt` (input file with flow logs)
- `lookup_table.csv` (input lookup table)
- The output is saved to `output.txt`.

To execute the program, use the following command:

```bash
./main
```
