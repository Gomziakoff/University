import random
import string

from pyexpat.errors import messages

import CIK
from Crypto.PublicKey import RSA
from Crypto.Signature.pkcs1_15 import PKCS115_SigScheme
from Crypto.Cipher import PKCS1_OAEP

candidates = ["Iosif Stalin","Noname","Blank"]


def registration():
    for voter in CIK.voter_list:
        if random.random() < 0.3:
            CIK.voter_list.remove(voter)
        else:
            pass

def vote():
    for voter in CIK.voter_list:
        private_key = RSA.generate(2048)
        public_key = private_key.publickey()
        B = random.choice(candidates)
        chipher = PKCS1_OAEP.new(public_key)
        Men,Ben = chipher.encrypt(str(voter['mark']).encode('utf-8')),chipher.encrypt(B.encode('utf-8'))
        message = (voter['mark'],Men,Ben)
        voter['public_key'] = public_key
        voter['private_key'] = private_key
        voter['message'] = message


