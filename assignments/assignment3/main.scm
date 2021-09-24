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

(define check-for-making
  (lambda (bst)
    (cond ((not (list? bst)) #f)
          ((null? bst) #t)
          (else (cond ((not (eq? 3 (length bst))) #f)
            ((not (list? (left bst))) #f)
            ((not (list? (right bst))) #f)
            (else #t))))))

(define check-elt
  (lambda (elt)
    (and (number? elt) (integer? elt))))

(define make-bst
  (lambda (elt left-bst right-bst)
    (if (and (check-for-making left-bst) (check-for-making right-bst) (check-elt elt))
      (cons elt (cons left-bst (cons right-bst '())))
      #f)))

(define preorder
  (lambda (bst)
    (cond ((null? bst) '())
          ((and (null? (left bst)) (null? (right bst)))    (cons (entry bst) '()))
          ((null? (left bst))                              (cons (entry bst) (preorder (right bst))))
          ((null? (right bst))                             (cons (entry bst) (preorder (left bst))))
          (else                                            (cons (entry bst) (append (preorder (left bst)) (preorder (right bst))))))))


(define inorder
  (lambda (bst)
    (cond ((null? bst) '())
          ((and (null? (left bst)) (null? (right bst)))    (cons (entry bst) '()))
          ((null? (left bst))                              (cons (entry bst) (inorder (right bst))))
          ((null? (right bst))                             (append (inorder (left bst)) (cons (entry bst) '())))
          (else                                            (append (inorder (left bst)) (cons (entry bst) (inorder (right bst))))))))


(define postorder
  (lambda (bst)
    (cond ((null? bst) '())
          ((and (null? (left bst)) (null? (right bst)))    (cons (entry bst) '()))
          ((null? (left bst))                              (append (postorder (right bst)) (cons (entry bst) '())))
          ((null? (right bst))                             (append (postorder (left bst)) (cons (entry bst) '())))
          (else                                            (append (append (postorder (left bst)) (postorder (right bst))) (cons (entry bst) '()))))))
