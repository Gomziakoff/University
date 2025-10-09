import CIK
import Voter

CIK.generate_voter_list(20)
Voter.registration()
CIK.show_voter_list()
CIK.generate_and_distribute_marks()
CIK.show_voter_list()
Voter.vote()
CIK.show_voter_list(True)
CIK.counter()