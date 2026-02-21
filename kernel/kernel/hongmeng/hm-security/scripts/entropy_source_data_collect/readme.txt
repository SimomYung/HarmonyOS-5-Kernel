Directory Info:
entropy_estimate_test: collect entropy estimate data from board
restart_test: collect entropy source data for restart test

How To Use:
1) clone hm-test, and checkout to dev-next branch
2) put hm-test with the script
3) run test script
   parameter:
	board_ip: board ip addr
	local_ip: local mathine ip addr
	user:     user name in local machine
	passwd:   user passwd in local machine
	entropy_count: test entropy source count (devhost sysmgr jitter), max value is 2

Notice: these scripts need to run in springboard machine, which has established
network connection with development board.
