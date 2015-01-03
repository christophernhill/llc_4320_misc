Useful bits of c code and shell scripts for extracting regions from global llc_4320
output on NASA Pleiades system. 

Contents
 
read_llc4320_hawaii_k.c - C program that extracts data from sub-region of llc_4320 files. Compile with gcc

example_of_sourced_commands_to_do_extract	- Example of shell commands to run the extract C program, taking input from
master store and writing extract files locally.

example_of_splitting_into_blocks_of_350	- Shows how to split long list of shell commands into blocks of 350. A block of 350 extract commands takes under two hours to run, which is the batch queue job length used. Extract needs to be dome from queue system, since front-end login nodes do not have fast access to master store file system.

example_pbs_command	- Example PBS command for submitting extract command file to PBS queue system.

example_pbs_job_file	- Example job file used in job submission that simply runs the long list of extract commands.

example_copy_back_to_eofe	- Example of scp back to local system. Requires ssh-agent to instantiate public key.
