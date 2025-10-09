from Crypto.Cipher import PKCS1_OAEP

import Voter
import random
import string
from Crypto.PublicKey import RSA
from Crypto.Signature.pkcs1_15 import PKCS115_SigScheme

voter_list = []
private_key, public_key = None, None
generated_marks = set()

#Цик публикует список избирателей
def generate_voter_list(num_voters):
    global voter_list

    for i in range(num_voters):
        voter_id = ''.join(random.choices(string.digits, k=5))
        voter_name = ''.join(random.choices(string.ascii_uppercase, k=5))
        voter_age = random.randint(18, 90)

        voter = {
            'id': voter_id,
            'name': voter_name,
            'age': voter_age,
        }
        voter_list.append(voter)


def generate_key_pair():
    global private_key, public_key
    private_key = RSA.generate(1024)
    public_key = private_key.publickey()

def show_voter_list(only_message = False):
    if only_message:
        for voter in voter_list:
            print(voter['message'])
    else:
        print(f"num of voters: {len(voter_list)}")
        for voter in voter_list:
            print(voter)



def generate_and_distribute_marks():
    for _ in range(len(voter_list)):
        generated = False
        while generated == False:
            random_mark = random.randint(10 ** 9, 10 ** 10 - 1)
            if random_mark not in generated_marks:
                generated_marks.add(random_mark)
                generated = True
    for voter,mark in zip(voter_list,generated_marks):
        voter['mark'] = mark

def counter():
    vote_count = {}
    for voter in voter_list:
        key = (voter['private_key'])
        cipher = PKCS1_OAEP.new(key)

        Bdec = cipher.decrypt(voter['message'][2])
        if str(Bdec) in vote_count:
            vote_count[str(Bdec)] += 1
        else:
            vote_count[str(Bdec)] = 1
    print("Результаты голосования",vote_count)




