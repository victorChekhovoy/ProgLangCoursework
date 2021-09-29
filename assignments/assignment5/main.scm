;; This function takes another function with 3 arguments as imput, and returns a "curried" version of it
(define curry3
  (lambda (func)
    (lambda (arg1)
      (lambda (arg2)
        (lambda (arg3)
          (func arg1 arg2 arg3))))))


;; This function takes a curried function with three arguments as input and returns an uncurried version of it
(define uncurry3
  (lambda (func)
    (lambda (arg1 arg2 arg3)
      (((func arg1) arg2) arg3))))


;; This function takes a curried function with any number of arguments as input
;; and returns an  uncurried version of it
(define uncurry
  (lambda (func)
    (lambda args
      (if (not (procedure? (func (car args))))
          (func (car args))
          (apply (uncurry (func (car args))) (cdr args))))))


(define my-filter
  (lambda (proc lst)
    (cond ((null? lst) '())
          ((proc (car lst))
            (cons (car lst) (my-filter proc (cdr lst))))
            (else (my-filter proc (cdr lst))))))


;; This is a helper function that takes a list as input and returns
;; another function. In turn, the returned function takes a value as input
;; and returns #t if the list doesn't contain the value and #f otherwise
(define not-contains?
  (lambda (list)
    (lambda (value)
      (cond ((null? list) #t)
            ((eq? (car list) value) #f)
            (else ((not-contains? (cdr list)) value))))))


;; This is a helper function that takes a list as input and returns
;; another function. In turn, the returned function takes a value as input
;; and returns #t if the list contains the value and #f if it doesn't
  (lambda (list)
    (lambda (value)
      (cond ((null? list) #f)
            ((eq? (car list) value) #t)
            (else ((contains? (cdr list)) value))))))


(define union
  (lambda (S1 S2)
    (cond ((null? S1) S2)
          ((null? S2) S1)
          (else (append (my-filter (not-contains? S1) S2) S1)))))


(define intersect
  (lambda (S1 S2)
    (cond ((null? S1) '())
          ((null? S2) '())
          (else (my-filter (contains? S1) S2)))))


(define exists
  (lambda(pred lst)
    (cond ((null? lst) #f)
          ((null? (my-filter pred lst)) #f)
          (else #t))))

















;hi
