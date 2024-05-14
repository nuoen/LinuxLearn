cmd_/home/nuoen/linux-6.7.12/kmodule/test1/modules.order := {   echo /home/nuoen/linux-6.7.12/kmodule/test1/mytest.ko; :; } | awk '!x[$$0]++' - > /home/nuoen/linux-6.7.12/kmodule/test1/modules.order
