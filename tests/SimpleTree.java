import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

/**
 * Created by danya on 10.03.17.
 */
public class SimpleTree {

    class Node {
        Integer key;
        Integer depth;
        Node left;
        Node right;
        Node parent;

        Node() {
            key = 0;
            depth = 1;
            left = null;
            right = null;
            parent = null;
        }

        Node(Integer key) {
            this.key = key;
            depth = 1;
            left = null;
            right = null;
            parent = null;
        }

        Node(Integer key, Node parent) {
            this.key = key;
            depth = parent.depth + 1;
            left = null;
            right = null;
            this.parent = parent;
        }

        Node(Node node) {
            this.key = node.key;
            this.left = node.left;
            this.right = node.right;
            this.parent = node.parent;
            this.depth = node.depth;
        }
    }

    class Pair_ {
        Integer resValue;
        private boolean isCorrect;

        Pair_() {
            resValue = 0;
            isCorrect = false;
        }

        Pair_(Integer resValue) {
            this.resValue = resValue;
            isCorrect = true;
        }

        public boolean getIsCorrect() {
            return isCorrect;
        }
    }

    Node root;

    SimpleTree() {
        root = null;
    }

    SimpleTree(Integer key) {
        root = new Node(key);
    }


    public void insert(Integer key, Node node) {
        if (root == null) {
            root = new Node(key);
            return;
        }
        //----меньшие элементы расположены слева, большие справа
        if (key < node.key) {
           if (node.left == null) {
               node.left = new Node(key, node);
           } else {
               insert(key, node.left);
           }
        } else if (key > node.key) {
           if (node.right == null) {
               node.right = new Node(key, node);
           } else {
               insert(key, node.right);
           }
       }
    }

    /**
     * Delete node from tree.
     * @param key
     * @param node
     * @return true, if node was success removed, else return false.
     */
    public boolean remove(Integer key, Node node) {

        //---левое поддерево:
        if (key < node.key) {
            //--смотрим левое поддерево:
            if (node.left != null && node.left.key.equals(key)) {
                //---нашли и оказался лист:
                if (node.left.left == null && node.left.right == null) {
                    //---удалаяем, затирая ссылку:
                    node.left = null;
                    return true;
                } else if (node.left.left == null && node.left.right != null) {
                    //---иначе, если один потомок, переподвешиваем
                    node.left.right.parent = node.left;
                    node.left = node.left.right;
                    return true;
                } else if (node.left.left != null && node.left.right == null) {
                    node.left.left.parent = node.left;
                    node.left = node.left.left;
                    return true;
                } else if (node.left.left != null && node.left.right != null) {
                    //---удаление узла с двумя сыновьями
                    Node tmp = new Node(node.left.right);
                    while (tmp.left != null) {
                        tmp = tmp.left;
                    }
                    Integer buffer = tmp.key;
                    remove(buffer, root);
                    node.left.key = buffer;
                    return true;
                }
            } else if (node.left != null) {
                //--т.е. если ключ меньше, чем левый ключ:
                return remove(key, node.left);
            } else if (node.left == null) return false; //----неудача
        }

        //---правое поддерево:
        if (key > node.key) {
            //--смотрим правое поддерево:
            if (node.right != null && node.right.key.equals(key)) {
                //---нашли и оказался лист:
                if (node.right.left == null && node.right.right == null) {
                    //---удалаяем, затирая ссылку:
                    node.right = null;
                    return true;
                } else if (node.right.left == null && node.right.right != null) {
                    node.right.right.parent = node.right;
                    node.right = node.right.right;
                    return true;
                } else if (node.right.left != null && node.right.right == null) {
                    node.right.left.parent = node.right;
                    node.right = node.right.left;
                    return true;
                } else if (node.right.left != null && node.right.right != null) {
                    //---удаление узла с двумя сыновьями
                    Node tmp = new Node(node.right.right);
                    while (tmp.left != null) {
                        tmp = tmp.left;
                    }
                    Integer buffer = tmp.key;
                    remove(buffer, root);
                    node.right.key = buffer;
                    return true;
                }
            } else if (node.right != null) {
                //--т.е. если ключ больше, чем левый ключ:
                return remove(key, node.right);
            } else if (node.right == null) return false; //---неудача
        }

        if (root.key.equals(key)) {
            if (root.right == null && root.left == null) {
                //---
                root = null;
                return true;
            } else if (root.left == null && root.right != null) {
                root.right.parent = null;
                root = root.right;
                return true;
            } else if (root.left != null && root.right == null) {
                root.left.parent = null;
                root = root.left;
                return true;
            } else if (root.left != null && root.right != null) {
                //---удаление узла с двумя сыновьями
                Node tmp = new Node(root.right);
                while (tmp.left != null) {
                    tmp = tmp.left;
                }
                Integer buffer = tmp.key;
                remove(buffer, root);
                root.key = buffer;
                return true;
            }
        }
        return false;
    }

    public boolean find(Integer key, Node node) {
        if (node == null) return false;

        if (node.key.equals(key)) return true;

        if (key < node.key) {
            if (node.left == null) {
                return false;
            } else {
                return find(key, node.left);
            }
        }

        if (key > node.key){
            if (node.right == null) {
                return false;
            } else {
                return find(key, node.right);
            }
        }

        return false;
    }

    public void printTree(Node node) {
        if (node == null) {
            System.out.println("Пустое дерево");
            return;
        }
        if (node.left != null)
            printTree(node.left);
        System.out.print("(" + node.key + ", " + node.depth + ") ");
        if (node.right != null)
            printTree(node.right);
        return;
    }

    public Pair_ next(Integer key) {
        //---если пустое
        if (root == null) return new Pair_();

        Node cur = root;
        Node res = null;

        while(cur != null) {
            if (cur.key > key) {
                res = cur;
                cur = cur.left;
            } else {
                cur = cur.right;
            }
        }
        return res == null ? new Pair_() : new Pair_(res.key);
    }

    public Pair_ prev(Integer key) {
        if (root == null) return new Pair_();
        Node cur = root;
        Node res = null;
        while(cur != null) {
            if (cur.key < key) {
                res = cur;
                cur = cur.right;
            } else {
                cur = cur.left;
            }
        }
        return res == null ? new Pair_() : new Pair_(res.key);
    }

    public void exec(String str, FileWriter outp) {

        String[] ops = str.split(" ");
        if (ops.length < 2) {
            return;
        }
        String op = ops[0];
        Integer value = Integer.valueOf(ops[1]);

        if (op.equals("insert")) {
            insert(value, root);
            return;
        }

        if (op.equals("delete")) {
            remove(value, root);
            return;
        }

        if (op.equals("exists")) {
            if (find(value, root)) {
                try {
                    outp.write("true\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                try {
                    outp.write("false\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return;
        }

        if (op.equals("prev")) {
            Pair_ tmp = prev(value);
            if (tmp.getIsCorrect()) {
                try {
                    outp.write(tmp.resValue + "\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                try {
                    outp.write("none\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return;
        }

        if (op.equals("next")) {
            Pair_ tmp = next(value);
            if (tmp.getIsCorrect()) {
                try {
                    outp.write(tmp.resValue + "\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                try {
                    outp.write("none\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return;
        }
    }

    public static void main(final String[] args) {
        SimpleTree tree = new SimpleTree();

        try {
            List<String> lines = Files.readAllLines(Paths.get("./input.txt"), StandardCharsets.UTF_8);
            FileWriter outp = new FileWriter("output_java.txt", false);
            for (String line : lines) {
                try{
                    tree.exec(line, outp);
                } catch (Exception ex) {
                    outp.write("EXCEPTION\n");
                    outp.flush();
                }
            }
            outp.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } 
    }
}
