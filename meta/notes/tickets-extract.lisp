"
Code to extract all the tickets from an unfuddle repository.  This
code is ugly, hackish, and will only work on my (bsummer4) machine.
"

"# Libs and Utility Functions/Macros"

(defun fuck (package)
  (let ((name (string-downcase (symbol-name package))))
    (asdf::load (format nil "/home/ben/opt/~a/~a.asd" name name)))
  (require package))
(defun fuck* (&rest packages) (mapc #'fuck packages))
(defun require* (&rest packages) (mapc #'require packages))
(fuck* :cl-walker :cl-syntax-sugar)
(require* :s-xml #|:spath|# :iterate)
(use-package :iterate)

(defmacro aif (c i &optional e)
  `(let ((it ,c))
     (if it ,i ,e)))

(defun last-element (list) (car (last list)))

(defun -- (list) (remove-if-not #'identity list))

(cl-syntax-sugar:set-lambda-with-bang-args-syntax-in-readtable
 :start-character #\[ :end-character #\])

"# XML"

(defun node? (node) (and (listp node) (keywordp (car node))))

(defun node-tag (node)
  (when (listp node)
    (first node)))

(defun node-attributes (node)
  (when (listp node)
    (let ((attributes (second node)))
     (when (and (listp attributes) (eq :@ (car attributes)))
       (cdr attributes)))))

(defun node-children (node)
  (when (listp node)
    (let ((attributes (second node))
         (children (cddr node)))
     "if there are no attributes then 'attributes is actually a child node.  "
     (if (and (listp attributes) (eq (car attributes) :@))
         children
         (cons attributes children)))))

(defvar *indent-level* 0)
(defun outline-node (node)
  (when node
    (when (node? node)
      (dotimes (i *indent-level*) (format t "  "))
      (format t "~a~%" (node-tag node)))
    (let ((*indent-level* (1+ *indent-level*)))
      (mapc #'outline-node (node-children node))))
  (values))


(defun find-subtrees (tag tree)
  (let ((result))
    (labels ((match? (subtree) (eq tag (node-tag subtree)))
             (loop* (subtree)
                (when (listp subtree)
                  (if (match? subtree)
                      (push subtree result)
                      (mapc #'loop* (cdr subtree))))))
      (loop* tree)
      (nreverse result))))


"# code specific to this file"

(defparameter *unfuddle*
  (s-xml:parse-xml-file "/home/ben/unfuddle.xml"
                        :output-type :sxml))

(defparameter *tags-we-care-about*
  '(:|assignee-id| :|created-at| :|description| :|id|
    :|reporter-id| :|resolution| :|resolution-description| :|status|
    :|summary| :|attachments| :|comments| :|body| :|author-id|))
(defparameter *author-usernames*
  '(1 "codeman655"
    2 "jingyuan"
    3 "jingyuan"
    4 "bsummer4"
    5 "jhoare"
    6 "dloftis"))

(defun tickets ()
  "I know from experimentation that the last <tickets> tag is the one
   I'm looking for."
  (last-element (find-subtrees :|tickets| *unfuddle*)))

;(defun messages ()
  ;find-subtrees :|message)

(defun extract-comment (comment)
  (-- (mapcar #'extract (node-children comment))))

;; todo special case for comments
(defun extract (ticket-child)
  "return a (name . value) or nil"
  (let ((tag (node-tag ticket-child))
        (children (node-children ticket-child)))
    (when (find tag *tags-we-care-about*)
      (aif (case tag
             (:|comments| (-- (mapcar #'extract-comment children)))
             (t (car children)))
           (cons tag it)))))

(defun fix (string) (string-trim " " (remove #\ string)))

(defmacro show (name)
  (assert (symbolp name))
  `(when ,name
     (format t "~a: ~a~%"
             ,(string-downcase (symbol-name name)) (fix ,name))))

(defun get-author (author)
  (and author
       (getf *author-usernames* (read-from-string author))))

(defun print-comments (comments)
  (iter (for comment in comments)
        (format t "~%%%~%")
        (labels ((field (symbol) (cdr (assoc symbol comment))))
          (let ((body (field :|body|))
                (date (field :|created-at|))
                (author (get-author (field :|author-id|))))
            (show author)
            (show date)
            (when body (format t "~%~a~%" (fix body)))))))

(defun output-ticket (ticket)
  (labels ((field (symbol) (cdr (assoc symbol ticket))))
    (let ((body (field :|description|))
          (comments (field :|comments|))
          (date (field :|created-at|))
          (id (field :|id|))
          (title (field :|summary|))
          (assigned-to (get-author (field :|assignee-id|)))
          (resolve-comment (field :|resolution-description|))
          (resolve (field :|resolution|))
          (author (get-author (field :|reporter-id|)))
          (status (field :|status|)))
      (print id)
      (with-open-file (out (format nil "/home/ben/tmp/tickets/~a" id)
                           :direction :output
                           :if-exists :overwrite
                           :if-does-not-exist :create)
        (let ((*standard-output* out))
          (show author)
          (show title)
          (show date)
          (show status)
          (show assigned-to)
          (when body (format t "~%~a~%" (fix body)))
          (when comments (print-comments comments))
          (when (or resolve resolve-comment)
            "We format the resolution message like a comment. "
            (let ((author author)
                  (title resolve)
                  (body resolve-comment))
              (format t "~%%%~%")
              (show author)
              (when (or body title)
                (when title (format t "~%resolved: ~a~%" title))
                (when body (format t "~%~a~%" body))))))))))

(defun extract-all ()
  (-- (iter (for ticket in (node-children (tickets)))
            (collect (iter (for data in (cdr ticket))
                           (for tag = (car data))
                           (for extract = (extract data))
                           (when extract
                             (collect extract)))))))

(mapc #'output-ticket (extract-all))
