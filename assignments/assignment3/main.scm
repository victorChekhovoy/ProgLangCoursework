;; This function takes any list as an argument and returns its length
(define length
  (lambda (l)
    (if (null? l)
        0
        (+ 1 (length (cdr l))))))

;; This function takes a binary search tree and checks if it satistfies
;; the requirements for entry, left, and right functions
(define bst_check
  (lambda (bst)
    (cond ((not (list? bst)) #f)
          ((not (equal? (length bst) 3)) #f)
          ((not (list? (car (cdr bst)))) #f)
          ((not (list? (car (cdr (cdr bst))))) #f)
          (else #t))))

;; This function takes a binary search tree as an argument, and returns
;; its root node's value if the tree is valid, and #f otherwise
(define entry
  (lambda (bst)
    (if (not (bst_check bst))
      #f
      (car bst))))

;; This function takes a binary search tree as an argument, and returns
;; the left subtree if the tree is valid, and #f otherwise
(define left
  (lambda (bst)
    (if (not (bst_check bst))
      #f
      (car (cdr bst)))))

;; This function takes a binary search tree as an argument, and returns
;; the right subtree if the tree is valid, and #f otherwise
(define right
  (lambda (bst)
    (if (not (bst_check bst))
      #f
      (car (cdr (cdr bst))))))

;; This functions takes a binary search tree as an argument, and returns #t
;; if the following are true:
;; The BST is a list, either null or has 3 elements, of which last two are lists.
;; Otherwise, returns #f
(define check-for-making
  (lambda (bst)
    (cond ((not (list? bst)) #f)
          ((null? bst) #t)
          (else (cond ((not (eq? 3 (length bst))) #f)
                ((not (list? (left bst))) #f)
                ((not (list? (right bst))) #f)
                (else #t))))))

;; This function takes the root node value of a binary search tree and returns
;; #t if it's a number and an integer, and #f other
(define check-elt
  (lambda (elt)
    (and (number? elt) (integer? elt))))

;; This function takes a root node value and two subtrees, and returns a new
;; binary search tree created with the given values. If the root node value is
;; not an integer, or if either subtree is either not a list or an invalid subtree, returns #f
(define make-bst
  (lambda (elt left-bst right-bst)
    (if (and (check-for-making left-bst) (check-for-making right-bst) (check-elt elt))
      (list elt left-bst right-bst)
      #f)))

;; This function takes a binary search tree as an argument and returns its elements
;; obtained from a preorder traversal
(define preorder
  (lambda (bst)
    (cond ((null? bst) '())
          ((and (null? (left bst)) (null? (right bst)))    (list (entry bst)))
          ((null? (left bst))                              (list (entry bst) (preorder (right bst)))
          ((null? (right bst))                             (list (entry bst) (preorder (left bst))))
          (else                                            (list (entry bst) (preorder (left bst)) (preorder (right bst))))))))

;; This function takes a binary search tree as an argument and returns its elements
;; obtained from an inorder traversal
(define inorder
  (lambda (bst)
    (cond ((null? bst) '())
          ((and (null? (left bst)) (null? (right bst)))    (list (entry bst)))
          ((null? (left bst))                              (list (entry bst) (inorder (right bst))))
          ((null? (right bst))                             (list (inorder (left bst)) (entry bst)))
          (else                                            (list (inorder (left bst)) (entry bst) (inorder (right bst)))))))

;; This function takes a binary search tree as an argument and returns its elements
;; obtained from a postorder traversal
(define postorder
  (lambda (bst)
    (cond ((null? bst) '())
          ((and (null? (left bst)) (null? (right bst)))    (list (entry bst)))
          ((null? (left bst))                              (list (postorder (right bst)) (entry bst)))
          ((null? (right bst))                             (list (postorder (left bst)) (entry bst)))
          (else                                            (list (postorder (left bst)) (postorder (right bst))(entry bst))))))

;; This function takes a value and a binary search tree, and returns a new BST
;; identical to the input but with the value inserted at the right spot
(define insert
  (lambda (v bst)
    (cond ((null? bst) (make-bst v '() '()))
          ((eq? v (entry bst)) bst)
          ((> v (entry bst))
            (if (null? (right bst))
              (list (entry bst) (left bst) (make-bst v '() '()))
              (list (entry bst) (left bst) (insert v (right bst)))))
          ((< v (entry bst))
            (if (null? (left bst))
              (list (entry bst) (make-bst v '() '()) (right bst))
              (list (entry bst) (insert v (left bst)) (right bst)))))))
