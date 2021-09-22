(define length
  (lambda (l)
    (if (null? l)
        0
        (+ 1 (length (cdr l))))))

(define bst_check
  (lambda (bst)
    (cond ((not (list? bst)) #f)
          ((not (equal? (length bst) 3)) #f)
          ((not (list? (car (cdr bst)))) #f)
          ((not (list? (car (cdr (cdr bst))))) #f)
          (else #t))))

(define entry
  (lambda (bst)
    (if (not (bst_check bst))
      #f
      (car bst))))

(define left
  (lambda (bst)
    (if (not (bst_check bst))
      #f
      (car (cdr bst)))))

(define right
  (lambda (bst)
    (if (not (bst_check bst))
      #f
      (car (cdr (cdr bst))))))



(define bst '(5 (3 () (4 () ()) ) ()))

(entry bst)
(left bst)
(right bst)
