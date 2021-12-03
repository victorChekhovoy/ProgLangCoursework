;;; Assignment 4, Viktor Chekhovoi


;; This function takes two integers as input, and returns a list with all
;; integers in the given range. If the range is invalid, returns an empty list
(define gen-list
  (lambda (start finish)
  (cond ((eq? start finish) (list start))
        ((> start finish) '())
        (else (cons start (gen-list (+ 1 start) finish))))))


;; This function takes a list of integers and an integer value as input,
;; and returns #t if the sum of any two neighboring elements in the list
;; is equal to the given value and #f otherwise
(define pair-sum?
  (lambda (list value)
  (cond ((null? list) #f)
        ((null? (car list)) #f)
        ((null? (cdr list)) #f)
        ((eq? value (+ (car list) (car (cdr list)))) #t)
        (else (pair-sum? (cdr list) value)))))


;; This function takes two integers as input, and returns a lazy list containing
;; all the integers in the given range
(define gen-lazy-list
  (lambda (start stop)
    (if (> start stop)
        #f
        (cons start
            (lambda () (gen-lazy-list (+ start 1) stop))))))


;; This function takes a lazy list of integers and an integer value as input,
;; and returns #t if the sum of any two neighboring elements in the list
;; is equal to the given value and #f otherwise
(define pair-sum-lazy?
  (lambda (list value)
  (cond ((null? list) #f)
        ((null? (car list)) #f)
        ((not ((cdr list))) #f)
        ((eq? value (+ (car list) (car ((cdr list))))))
        (else (pair-sum-lazy? ((cdr list)) value)))))


;; This function takes a list and returns a lazy version of that list
(define make-lazy
  (lambda (list)
    (if (null? list)
        #f
        (cons (car list) (lambda () (make-lazy (cdr list)))))))
