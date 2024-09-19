#
# Change the HW variable to match the homework
#
NUM			 = 1

HW	  = hw$(NUM)
CLASS   = /homes/cs240
PROJ	= $(CLASS)/public/homework/$(HW)

.PHONY: Makefile
Makefile : $(PROJ)/Makefile
		rm -f Makefile
		cp $(PROJ)/Makefile .

#
# Extra targets to do interesting things...
#

submit: Makefile
		~cs240/bin/safe-repo $(HW)
		git checkout master >> .local.git.out || echo
		git add *.c >> .local.git.out || echo
		git add *.h >> .local.git.out || echo
		git commit -a -m "Submission for homework $(HW)" >> .local.git.out || echo
		git push origin master

verify: Makefile
		~cs240/bin/safe-repo $(HW)
		~cs240/bin/sub-verify $(HW)
