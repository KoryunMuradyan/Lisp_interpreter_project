(defun foo(a)(+ 111111))
(defun foo_1(a b)(print a)(foo_1 7 9)) 
(print (- 16 8))
(print (+ 777 223 (- 300 400)))
(setq c 777)
(foo_1 3 c)
(setq ll 77)
(print (+ ll 23))
(if ( < ll 17)(print "less than 17")(print "greater then 17"))
(print (foo 4))
