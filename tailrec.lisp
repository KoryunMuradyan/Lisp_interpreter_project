   (defun f (x y)
       (cond
           ((< x 1) y)
           (t (f (- x 1) (+ y y)))))





(defun append (L1 L2)
    (cond
        ((not (listp L1)) nil)
        ((not (listp L2)) nil)
        ((null L2) L1)
        (t (appendReverse (reverse L1) L2))))






(defun appendReverse (L1 L2)
    (cond
        ((null L1) L2)
        (t (appendReverse (cdr L1) (cons (car L1) L2)))))






(defun reverse (L)
   (cond
       ((not (listp L)) nil)
       (t (reverseApp L '()))))






(defun reverseApp (L soFar)
   (cond
       ((null L) soFar)
       (t (reverseApp (cdr L) (cons (car L) soFar)))))





(defun length (L)
   (cond
       ((not (listp L)) nil)
       (t (lengthAcc L 0))))






(defun lengthAcc (L soFar)
   (cond
       ((null L) soFar)
       (t (lengthAcc (cdr L) (+ 1 soFar)))))


(defun min (L)
   (cond
       ((not (listp L)) nil)
       ((null L) nil)
       ((not (numberp (car L))) nil)
       (t (minSoFar (cdr L) (car L)))))







(defun minSoFar (L prevMin)
   (cond
       ((null L) prevMin)
       ((not (numberp (car L))) nil)
       ((< prevMin (car L)) (minSoFar (cdr L) prevMin))
       (t (minSoFar (cdr L) (car L)))))





(defun fib (N)
   (cond
      ((not (integerp N)) nil)
      ((< N 3) 1)
      (t (fibAcc N 3 1 1))))





(defun fibAcc (N NewN prevN lastN)
   (cond
      ((< N NewN) lastN)
      (t (fibAcc N (+ NewN 1) lastN (+ prevN lastN)))))

