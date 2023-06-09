public protocol ArbBinTProtocol {
    associatedtype T
    // Check if tree is empty
    /// - returns: `true` if the root references no node, `false` otherwise
    func ab_vide() -> Bool
    // Get the info stored at the node
    /// - returns: the element stored in the node
    /// - throws: `fatalerror` if the tree is empty
    func racine() throws -> T
    // Get the left subtree
    /// - returns: a PBTree corresponding to the left subtree of the current tree
    /// - throws: `fatalerror` if the tree is empty
    func sag() throws -> Self
    // Get the right subtree
    /// - returns: a PBTree corresponding to the right subtree of the current tree
    /// - throws: `fatalerror` if the tree is empty
    func sad() throws -> Self
}
// ----------------------------------
enum ArbBinError : Error {
    case pasDeSagDsArbreVide
    case pasDeSadDsArbreVide
    case pasdInfoDsArbreVide
}
// ----------------------------------
enum ArbBin<T : Comparable> : CustomStringConvertible, ArbBinTProtocol {
    case vide   
    indirect case noeud(ArbBin, T, ArbBin)
    func ab_vide() -> Bool {
        //return self == ArbBin<T>.vide 
        if case ArbBin.vide = self {return true} else {return false}
    }
    public var description: String { return descDecalRec(dec : "") }
    // Fonction auxiliaire pour faire des décalages et montrer les niveaux de l'arbre
    func descDecalRec(dec : String) -> String {
        switch self {
        case let .noeud(left, value, right):
            return right.descDecalRec(dec:dec+"\t") + dec+"(\(value))\n" + left.descDecalRec(dec:dec+"\t")
        case .vide: return ""
        }
    }
    func feuille() -> Bool {
      switch self {
        case .vide: return false
        case let .noeud(.vide, _, .vide): return true
        default : return false
        }
    }
    // A traversal for counting nodes in the tree:
    /// - returns: the number of nodes in the tree, 0 if tree is empty
    var count: Int {
      switch self {
        case let .noeud(sag, _, sad):
            return sag.count + 1 + sad.count
        case .vide:
            return 0
        }
    }
    // ACCESSEURS :
    // Return left subtree
    /// - throws: Error in case tree is empty
    func sag() throws -> ArbBin<T>  {
        switch self {
        case .vide: throw ArbBinError.pasDeSagDsArbreVide
        case let .noeud(left,_,_): return left
        }
    }
    // Return right subtree
    /// - throws: Error in case tree is empty
    func sad() throws -> ArbBin<T>  {
        switch self {
        case .vide: throw ArbBinError.pasDeSadDsArbreVide
        case let .noeud(_,_,right): return right
        }
    }
    // Return value stored at root
    /// - throws: Error in case tree is empty
    func racine() throws -> T {
        switch self {
        case .vide: throw ArbBinError.pasdInfoDsArbreVide
        case let .noeud(_,val,_): return val
        }
    }
}

// -----------------------------------------------------
// ---      PARTIE 2  -   modifications d'un arbre   ---
// -----------------------------------------------------
/* Ok, faisons un récap :
 - on a défini un type qui est un mix de type valeur et de type référence (un 'enum' avec des parties 'indirect')
 - ce type vérifie les spécifications fonctionnelles demandées (dans le protocol)
 
 Maintenant on veut modifier l'arbre (la plupart des applications ont besoin d'une structure de données qui évolue dynamiquement !)
 */

// Ajoutons une fonction pour auditer les adresses des variables :
extension ArbBin {
    func printAdresses() -> String  {
        if self.feuille() {return String(unsafeBitCast(self, to: Int.self)) }
        switch self {
        case .vide: return ""
        case let .noeud(left, _, right):
            var ret : String = "("+left.printAdresses()+","
            //ret = ret + String(format: "%p",&self)
            let addrSelf = unsafeBitCast(self, to: Int.self) // ok for values but not sure prints the right thing
            //let addrSelf = Unmanaged.passUnretained(self).toOpaque() // only for reference types
            ret += String(addrSelf)+","+right.printAdresses()+")"
            return ret
        }
      }
}
// ----------------------------------
// On construit maintenant un arbre à la main :
print("\nDéfinissons un arbre sur des entiers :")
// 1) définissons des feuilles :
let f1 = ArbBin.noeud(.vide, 4, .vide)
let f2 = ArbBin.noeud(.vide, 6, .vide)
let f3 = ArbBin.noeud(.vide, 5, .vide)
// 2) puis des noeuds internes pour la gauche de l'arbre

var leftSubtree = ArbBin.noeud(f1, 3, f2)
var treeTest = ArbBin.noeud(leftSubtree, 10, f3)

print("On a construit :\n \(treeTest)")
print(treeTest.printAdresses())
var secondA = treeTest
print("Adresses de secondA : "+secondA.printAdresses())


// 3) Essayons de changer le sous-arbre gauche :
print("Adresses de leftSubtree : "+leftSubtree.printAdresses())
leftSubtree = ArbBin.noeud(.vide, 1, .vide)
print("Adresses apres de leftSubtree : "+leftSubtree.printAdresses())

print("Sous-arbre modifié : \n\(leftSubtree)")

print("Arbre modifié ? : \n\(treeTest)")
// -----------------------------------------------------
/*
 Mince ! L'arbre n'a pas été modifié quand on a modifié son sous-arbre...
 Question : pouvez-vous dire pourquoi ? */
// Réponse : ...
// -----------------------------------------------------
// Pour éviter ce soucis, essayons de modifier la variable contenant l'arbre elle-même :

//    ... on obtient une erreur <- Cannot assign to ArbBin<T>... Pourquoi ?
// Réponse : ...

// Ne fonctionne pas non plus :
//  var treeTest.sag = leftSubtree
// Pourquoi ? 
// Réponse : ...

extension ArbBin {
    // Replace value contained at root of (sub)tree:
    mutating func setRacine(newValue : T) {
      guard case let .noeud(sag, _ , sad) = self else {
            throw ArbBinError.pasdInfoDsArbreVide}
        // on remplace le noeud par le même mais avec la valeur changée
        self = .noeud(sag, newValue, sad)
    }
}
        
print("Avant modif° : " + treeTest.printAdresses())
do {
    try treeTest.setRacine(newValue : 50) 
    //try treeTest.setSag(newValue : ArbBin.vide)
   // try treeTest.setSad(newValue : ArbBin.noeud(.vide, 5, .vide))
}
catch (ArbBinError.pasdInfoDsArbreVide) {
  print("L'arbre est vide : pas de noeud à changer")
}  