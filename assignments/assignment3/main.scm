(define length
  (lambda (l)
    (if (null? l)
        0
        (+ 1 (length (cdr l))))))

(define bst_check
  (lambda (bst)
    (cond ((not (list? bst)) #f)
          ((not (equal? (length bst) 3)) #f)
          ((not (number? (car bst))) #f)
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

(define make-bst
  (lambda (elt left-bst right-bst)
    (cond ((not (number? elt)) #f)
          ((not (list? left-bst)) #f)
          ((not (list? right-bst)) #f))

    (if (eq? (length left-bst) 3)
      (cond ((not (list? (car (cdr left-bst)))) #f)
            ((not (list? (car (cdr (cdr left-bst))))) #f))
      (if (not (null? left-bst))
        #f))
    (if (eq? (length right-bst) 3)
       (cond ((not (list? (car (cdr right-bst)))) #f)
             ((not (list? (car (cdr (cdr right-bst))))) #f))
       (if (not (null? right-bst))
         #f))



    ;cons elt (cons left-bst (cons right-bst '())))))


(make-bst 5 '() '())
