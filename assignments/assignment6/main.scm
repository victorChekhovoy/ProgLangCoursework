;;; Assignment 6 by Viktor Chekhovoi
;; “If anyone copies this assignment, let him die the death;
;; let him be fried in a pan; let the falling sickness and fever seize him;
;; let him be broken on the wheel, and hanged. Amen.”
;; - Adaped medieval library curse

;; This function takes two numbers as input,
;; and returns a lazy list containing the numbers between the two given integers
;; if possible, otherwise returns #f
(define seq
  (lambda (first last)
    (if (> first last)
      #f
      (cons first (lambda () (seq (+ 1 first) last))))))


;; This function takes starting number as input and returns a lazy list containing
;; an infinite sequence of numbers starting on that integer
(define inf-seq
    (lambda (first)
      (cons first (lambda () (inf-seq (+ 1 first))))))


;; This function takes a lazy list and a number as input,
;; and returns the first n elements of a lazy list, and #f if it's not possible
(define first-n
  (lambda (lazy-list n)
    (cond ((or (null? lazy-list) (eq? lazy-list #f)) '())
          ((eq? ((cdr lazy-list)) #f) (list (car lazy-list)))
          ((eq? n 1) (list (car lazy-list)))
          (else (cons (car lazy-list) (first-n ((cdr lazy-list)) (- n 1)))))))


;; This function takes a lazy list and a number as input,
;; and returns the nth element of the list if possible, otherwise returns #f
(define nth
  (lambda (lazy-list n)
    (cond ((eq? lazy-list #f) #f)
          ((eq? n 1) (car lazy-list))
          ((eq? n 0) #f)
          (else (nth ((cdr lazy-list)) (- n 1))))))


;; This function takes a lazy list and an integer as input,
;; and removes every multiple of the given number from the list
(define filter-multiples
  (lambda (lazy-list n)
    (cond ((eq? lazy-list #f) #f)
          ((eq? (modulo (car lazy-list) n) 0) (filter-multiples ((cdr lazy-list)) n))
          (else (cons (car lazy-list) (lambda () (filter-multiples ((cdr lazy-list)) n)))))))


;; This function takes a lazy list as input, and returns the first element
;; of the list followed by the rest of the elements that aren't divisible by the first
(define almost-eratosthenes
  (lambda (lazy-list)
    (cons (car lazy-list) (lambda () (filter-multiples lazy-list (car lazy-list))))))


;; This function takes a lazy list as input and implements
;; the sieve of Eratosthenes: returns a lazy list of all the prime numbers in the list
(define eratosthenes
  (lambda (lazy-list)
    (cond ((eq? lazy-list #f) #f)
          (else (cons (car lazy-list) (lambda () (eratosthenes (filter-multiples ((cdr lazy-list)) (car lazy-list)))))))))


;; This function generates an infinite lazy list containing only prime numbers
(define primes
  (lambda ()
    (eratosthenes (inf-seq 2))))
