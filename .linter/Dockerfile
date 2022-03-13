# vi: filetype=dockerfile
FROM alpine:latest

RUN apk --update add \
    clang-extra-tools \
    && \
  rm -rf /var/lib/apt/lists/* && \
  rm /var/cache/apk/*

ADD entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]
