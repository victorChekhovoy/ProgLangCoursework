;; Carleton College, CS 251 Fall 2021
;; Assignment 5
;; Alia Babinet and Viktor Chekhovoi

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

;; This function takes in a procedure and a list and returns a list that contains
;; elements from the original list for which procedure is true in the order they
;; were in the original list
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

;; This function takes in two sets and returns the union of them. That is to say
;; the set that contains all items from both lists (without duplicates)
(define union
  (lambda (S1 S2)
    (cond ((null? S1) S2)
          ((null? S2) S1)
          (else (append (my-filter (not-contains? S1) S2) S1)))))

;; This function takes in two sets and returns the intersect of them. That is to
;; say the set that contains only the items that both sets have in common
(define intersect
  (lambda (S1 S2)
    (cond ((null? S1) '())
          ((null? S2) '())
          (else (my-filter (contains? S1) S2)))))

;; This function takes in a predicate and a list and returns #t if there is at
;; least one item in the list for which the predicate is true. Otherwise it
;; returns #f
(define exists
  (lambda (pred lst)
    (cond ((null? lst) #f)
          ((null? (my-filter pred lst)) #f)
          (else #t))))
