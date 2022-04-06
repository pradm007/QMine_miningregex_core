FROM ubuntu:18.04

LABEL "qmine_engine_pradeep_mahato" "1.0"

WORKDIR /usr/local

RUN apt update && \
    apt install --silent -y nodejs npm redis-server libboost-all-dev git vim ragel software-properties-common wget htop && \
    wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz && tar -zxvf cmake-3.15.2.tar.gz && cd cmake-3.15.2 && ./bootstrap && make && make install
RUN git clone https://github.com/redis/hiredis.git && cd hiredis && \
    make && make install && cd ..
RUN git clone https://github.com/sewenew/redis-plus-plus.git && \
    cd redis-plus-plus && mkdir compile && \
    cd compile && cmake -DCMAKE_BUILD_TYPE=Release .. && make && make install
    
RUN touch /var/log/qmine_c.out

WORKDIR /home

RUN pwd && git clone https://github.com/pradm007/QMine_miningregex_core.git && cd QMine_miningregex_core && \
     git checkout redis_threadable

WORKDIR /home/QMine_miningregex_core

RUN make server


# CMD ["make", "server"]
# CMD ["./home/QMine_miningregex_core/bin/main server"]
CMD ./home/QMine_miningregex_core/bin/main server
EXPOSE 6379


# WORKDIR /usr/local
# RUN apt-get install -y software-properties-common wget
# RUN wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz && tar -zxvf cmake-3.15.2.tar.gz && cd cmake-3.15.2 && ./bootstrap && make && make install

# RUN git clone https://github.com/redis/hiredis.git

# WORKDIR /usr/local/hiredis
# RUN make
# RUN make install

# WORKDIR /usr/local
# RUN git clone https://github.com/sewenew/redis-plus-plus.git
# WORKDIR /usr/local/redis-plus-plus
# RUN mkdir compile
# WORKDIR /usr/local/redis-plus-plus/compile
# RUN cmake -DCMAKE_BUILD_TYPE=Release ..
# RUN make
# RUN make install

# WORKDIR /home

# RUN touch /var/log/qmine_c.out
# RUN touch /var/log/qmine_node.out

# RUN git clone https://github.com/pradm007/QMine_miningregex_core.git

# RUN apt install -y htop 
# WORKDIR /home/QMine_miningregex_core
# RUN git checkout redis_threadable


# # RUN nohup redis-server &
# # RUN nohup make server > /var/log/qmine_c.out &
# WORKDIR /home

# RUN git clone https://github.com/pradm007/QMineWebApp.git
# WORKDIR /home/QMineWebApp
# RUN git checkout dev && npm install --silent
# RUN npm install --silent -g pm2 frontail
# # RUN pm2 start app.js -l /var/log/qmine_node.out
# RUN ls
# RUN chmod 777 /home/QMineWebApp/start_script.sh

# # RUN npm i frontail --quiet -g
# EXPOSE 5000

# # CMD frontail /var/log/qmine_c.out /var/log/qmine_node.out
# WORKDIR /home

# ENTRYPOINT "/bin/bash"

# # CMD ["sh", "/home/QMineWebApp/start_script.sh"]
# CMD ./home/QMineWebApp/start_script.sh