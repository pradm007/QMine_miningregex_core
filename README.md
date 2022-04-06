# MiningRegex__Core

This is the central core for mining regular expression in a trace file.
Version : 1.0.1

### How do I get set up? ###

Execute the following to install necessary dependencies:
```
  $ sudo apt install libboost-all-dev
```
Read [this](#How-to-run-tests) for execution.

### Configuration

Using the make file, one can change configurations for the mining software. Following are the flags that can be used for configuration :
* THREADS : *Count of thread :*
* CSVOUTPUT : *Flag for enabling/disabling CSV output. Takes 0/1*
* DISPLAY_MAP : *Flag for enabling/disabling dumping mined pattern output in stdout*

### Dependencies
* [GCC 9.3+](https://gcc.gnu.org/)
* [OpenMP 5.0](https://www.openmp.org/)

### How to run tests
Basic commands can be broken into two parts: generating synthetic testcase and mining for valid match.

##### Test Case Generation
Generates the necessary synthetic test case (T-QRET).
```
make tracegen
```
This command will generate 1 consolidated file : 
* Event and/or Quantifiable integer

##### Data Mining with timed constraint
Upon execution, the user must enter their time bounded regex pattern (T-QRE) along with the alphabet size.
```
make main
```
### To be updated soon
* Summary of set up
* Database configuration
* Deployment instructions

### Contribution guideline
Please contact the original author for possible contribution/raising bug. [To be updated soon]

### Code review
For addressing any issue or providing feature support. Please follow [contribution guidline](###Contribution-guideline) and raise a pull request.

### Who do I talk to?

Please raise an issue if noticed and we would get back to you asap.

### License
M.I.T