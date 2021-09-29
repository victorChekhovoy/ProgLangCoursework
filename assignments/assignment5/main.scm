(define curry3
  (lambda (func)
    (lambda (arg1)
      (lambda (arg2)
        (lambda (arg3)
          (func arg1 arg2 arg3))))))

(define uncurry3
  (lambda (func)
    (lambda (arg1 arg2 arg3)
      (((func arg1) arg2) arg3))))

(define uncurry
  (lambda (func)
    (lambda args
      (if (not (procedure? (func (car args))))
          (func (car args))
          (apply (uncurry (func (car args))) (cdr args))))))





(define curry-plus
    (lambda ()
        (lambda (x)
            (lambda (y)
                (+ x y)))))

(define curry-plus-four
    (lambda ()
        (lambda (x)
            (lambda (y)
              (lambda (z)
                  (lambda (w)
                    (+ x y z w)))))))

((uncurry (curry-plus)) 3 5)

((uncurry (curry-plus-four)) 1 3 5 10)

(define my-filter
  (lambda (proc lst)
    (cond ((null? lst) '())
          ((proc (car lst))
            (cons (car lst) (my-filter proc (cdr lst))))
            (else (my-filter proc (cdr lst))))))

(my-filter odd? '(1 2 3 4)) ;--> (1 3)
; does it need to be in order?
(define union
  (lambda (S1 S2)
    (cond ((null? S1) S2)
          ((null? S2) S1)
          (my-filter (member S1) S2)
          (union S1 (cdr S2))
            (else(union (cons (car S2) S1) (cdr S2))
            )))
